/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:02:42 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/28 16:55:37 by cmaubert         ###   ########.fr       */
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

char	**fill_cmd_tab(char *cmd, t_token *current, char **split_cmd) 
{
	split_cmd = ft_calloc(4, sizeof(char *));
	if (!split_cmd)
		return (NULL);	
	split_cmd[0] = cmd;
	if (current->next)
		current = current->next;	
	if (current->type == OPTION || current->type == ARGUMENT)
		split_cmd[1] = current->value;
	else
		split_cmd[1] = NULL;	
	if (current->next)
		current = current->next;
	if (current->type == OPTION || current->type == ARGUMENT)
		split_cmd[2] = current->value;
	else 
		split_cmd[2] = NULL;
	split_cmd[3] = NULL;
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
		// dprintf(2, "path = %s, split_cmd = %s, %s, %s, %s\n", path, split_cmd[0], split_cmd[1], split_cmd[2], split_cmd[3]);
		// dprintf(2, "split_cmd = %s, %s, %s, %s\n", split_cmd[0], split_cmd[1], split_cmd[2], split_cmd[3]);
		path = get_path_and_check(&split_cmd[0], p->envp);
		if (execve(path, split_cmd, p->envp) == -1)
			free(path);
	}
	ft_free_tab(split_cmd);
	perror("exec");
	return (-1);
}
int	handle_input_redirection(t_pipex *p, char *heredoc)
{
	int	fd_in;
	
	fd_in = -1;
	if (heredoc)
		fd_in = open(heredoc, O_RDONLY | 0644);
	else
		fd_in = open(p->token->next->value, O_RDONLY | 0644);
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
	int	fd_out;
	fd_out = -1;
	if (current->type != PIPEX)
	{
		if (current->type == REDIRECT_OUT)
			fd_out = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (current->type == APPEND_OUT)
			fd_out = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

int	simple_cmd(t_pipex *p, char *heredoc)
{
	pid_t		pid;
	t_token	*current;
	int		status;
	int		fd_in;
	// int		exit_code;
	// is_infile = 0;
	
	current = p->token;
	pid = fork();
	if (pid == - 1)
		ft_error("fork");
	else if (pid == 0)
	{
		if (p->token->type == REDIRECT_IN || p->token->type == HEREDOC)
			fd_in = handle_input_redirection(p, heredoc);
		while (current->next && current->type != REDIRECT_OUT && current->type != APPEND_OUT)
			current = current->next;
		handle_output_redirection(current, p, fd_in);
		while (current->prev && current->type != ARGUMENT)
		{  // il va falloir changer le moyen d'acceder aux commandes en cas d'argument "wc -l" + commandes multiples sans pipe "cat cat cat"
			dprintf(2, "current->value = %s\n", current->value);
			current = current->prev;
		}
		execute(current->value, current, p);
	}
	waitpid(pid, &status, 0);
	// return (ft_wait(pid));
	return (0);
}
