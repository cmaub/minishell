/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:02:42 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/29 15:12:28 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
/*
execve takes 3 arguments 1)path of the cmds 2)a char** with the cmd and options 3)env
in this function we create the second argument
- split_cmd[0] = cmd
- split_cmd[1] = option if exist or argument if exist and no option
- split_cmd[2] = argument if exist
- split_cmd[3] = NULL
*/ 

char	**fill_cmd_tab(char *cmd, t_token  *current, char **split_cmd) 
{
	split_cmd = ft_calloc(4, sizeof(char *));
	if (!split_cmd)
		return (NULL);	
	split_cmd[0] = cmd;
	if (current->prev && current->prev->type == FILENAME)
	{
		if ((current->next && current->next->type != ARGUMENT)/* || !current->next*/)
		{
			dprintf(2, "condition remplie\n");
			split_cmd[1] = current->prev->value;
		}
	}
	/* ATTENTION : si plusieurs cmd se suivent, le bash execute la premiere + msg d'erreur */
	if (current->next)
	{
		current = current->next;
		if (current->type == ARGUMENT || current->type == FILENAME)
			split_cmd[1] = current->value;
		else
		{
			// split_cmd[1] = NULL;	
			current = current->next;
			if (current->type == ARGUMENT 
				|| (current->type == FILENAME && current->prev->type != APPEND_OUT && current->prev->type != REDIRECT_OUT))
				split_cmd[2] = current->value;
		}
	}
	return (split_cmd);
}


/*
- create a **char with the commands and its option / arguments
- search for the path of the command
- execute the cmd with its option and arguments
*/

int	execute(char *cmd, t_token *current, t_pipex *p)
{
	char	**split_cmd;
	char	*path;

	split_cmd = NULL;
	split_cmd = fill_cmd_tab(cmd, current, split_cmd);
	dprintf(2, "sortie de la fonction fill cmd tab\n");
	if (ft_strchr(cmd, '/') || no_envp(p->envp))
	{
		if (access(split_cmd[0], F_OK | R_OK) == -1)
		{
			ft_free_tab(split_cmd);
			perror("access");
			return (-1);
		}
		execve(split_cmd[0], split_cmd, NULL);
	}
	else if (!ft_strchr(cmd, '/') && !no_envp(p->envp))
	{
		path = get_path_and_check(&split_cmd[0], p->envp);
		dprintf(2, "path = %s, split_cmd = %s, %s, %s, %s\n", path, split_cmd[0], split_cmd[1], split_cmd[2], split_cmd[3]);
		if (execve(path, split_cmd, p->envp) == -1)
		{
			split_cmd[1] = NULL;
			
			free(path);
		}
	}
	ft_free_tab(split_cmd);
	perror("exec");
	return (-1);
}
int	handle_input_redirection(t_pipex *p, t_token *current, char *heredoc)
{
	int	fd_in;
	dprintf(2, " INPUT ===== current->value = %s\n", current->value);
	fd_in = -1;
	if (heredoc)
		fd_in = open(heredoc, O_RDONLY | 0644);
	else
		fd_in = open(current->next->value, O_RDONLY | 0644);
	if (fd_in == -1)
	{
		safe_close(p->pipefd[0]);
		safe_close(p->pipefd[1]);
		ft_error("open");
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		safe_close(p->pipefd[0]);
		safe_close(p->pipefd[1]);
		safe_close(fd_in);
		ft_error("dup2");
	}
	safe_close(fd_in);
	return (fd_in);
}
/*
This function opens fd_out or stops if no output redirection
then dup2 and close fd_out
*/
void	handle_output_redirection(t_token *current, t_pipex *p, int fd_in)
{
	dprintf(2, "entree dans output redirection\n");
	dprintf(2, "current->value = %s\n", current->value);
	int	fd_out;
	fd_out = -1;
	if (current->type != PIPEX)
	{
		if (current->prev->type == REDIRECT_OUT)
			fd_out = open(current->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (current->prev->type == APPEND_OUT)
			fd_out = open(current->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			return ;
		if (fd_out == -1) // fermer les fd ouverts precedemment
		{
			safe_close(p->pipefd[0]);
			safe_close(p->pipefd[1]);
			ft_error("open");
		}
		if (dup2(fd_out, STDOUT_FILENO) == -1) 
		{
			dprintf(2, "Hey output file \n");
			safe_close(p->pipefd[0]);
			safe_close(p->pipefd[1]);
			safe_close(fd_out);	
			ft_error("dup2");
		}
	}
	else
	{
		if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
		{
			dprintf(2, "Hey output pipe \n");
			ft_close_error(&fd_in, p, "dup2");
		}
	}
	safe_close(fd_out);
}
// separer struct t_token et t_pipex plus tard ?
int	simple_cmd(t_pipex *p, char *heredoc)
{
	pid_t		pid;
	t_token	*current;
	// t_token	*start;
	int		status;
	int		fd_in;
	// int		exit_code;
	// is_infile = 0;
	
	current = p->token;
	// start = current;
	pid = fork();
	if (pid == - 1)
		ft_error("fork");
	else if (pid == 0)
	{
		
		while (current->prev && current->type != REDIRECT_IN && current->type != HEREDOC)
			current = current->prev;
		dprintf(2, "pid 0 === current->type = %d\n", current->type);
		if (current->type == REDIRECT_IN || current->type == HEREDOC)
		{
			fd_in = handle_input_redirection(p, current, heredoc);
		}
		while (current->next && current->type != REDIRECT_OUT && current->type != APPEND_OUT)
			current = current->next;
		dprintf(2, "current->value = %s\n", current->value);
		handle_output_redirection(current, p, fd_in);
		
		while (current->prev)
		{
			current = current->prev;
		}
		while (current->next && current->type != ARGUMENT)
		{  // il va falloir changer le moyen d'acceder aux commandes en cas d'argument "wc -l" + commandes multiples sans pipe "cat cat cat"
			
			// if (current->prev && current->prev->type == ARGUMENT && current->type == ARGUMENT)
			// {
			// 	current = current->prev;
			// 	dprintf(2, "current->prev->type == ARGUMENT %s\n", current->prev->value);
			// }
			current = current->next;
			// dprintf(2, "current->value = %s\n", current->value);
		}
		if (current->type == ARGUMENT)
		{
			execute(current->value, current, p);
		}
		// dprintf(2, "current->value = %s\n", current->value);
		
	}
	waitpid(pid, &status, 0);
	// return (ft_wait(pid));
	return (0);
}
