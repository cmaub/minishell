/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:34:09 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/18 13:14:39 by anvander         ###   ########.fr       */
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
		
	if (current->type == OPTION || current->type == ARGUMENT || current->type == TBD || current->type == FILENAME)
		split_cmd[1] = current->value;
	else
		split_cmd[1] = NULL;	
	
	if (current->next)
		current = current->next;
	if (current->type == OPTION || current->type == ARGUMENT || current->type == TBD || current->type == FILENAME)
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
void	execute(char *cmd, t_token *current, t_pipex *p)
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
			exit (EXIT_FAILURE);
		}
		execve(split_cmd[0], split_cmd, NULL);
	}
	else if (!ft_strchr(cmd, '/') && !no_envp(p->envp))
	{
		path = get_path_and_check(&split_cmd[0], p->envp);
		if (execve(path, split_cmd, p->envp) == -1)
			free(path);
	}
	ft_free_tab(split_cmd);
	perror("exec");
	exit (EXIT_FAILURE);
}

int	handle_input_redirection(t_pipex *p)
{
	int	fd_in;

	fd_in = open(p->token->next->value, O_RDONLY | 0644);
	if (fd_in == -1)
		ft_error("open");
	if (dup2(fd_in, STDIN_FILENO) == -1)
		ft_error("dup2");
	safe_close(fd_in);
	return (1);
}

/*
This function opens fd_out or stops if no output redirection
then dup2 and close fd_out
*/
void	handle_output_redirection(t_token *current)
{
	int	fd_out;

	if (current->type == REDIRECT_OUT)
		fd_out = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (current->type == APPEND_OUT)
		fd_out = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return ;
		
	if (fd_out == -1)
		ft_error("open");
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		ft_error("dup2");
	safe_close(fd_out);
}

int	simple_cmd(t_pipex *p)
{
	pid_t		pid;
	t_token	*current;
	// int		exit_code;
	int		is_infile;
	

	is_infile = 0;
	current = p->token;
	pid = fork();
	if (pid == - 1)
		ft_error("fork");
	else if (pid == 0)
	{
		if (p->token->type == REDIRECT_IN)
			is_infile = handle_input_redirection(p);
		/* ATTENTION AU HEREDOC*/
		while (current->next && current->type != REDIRECT_OUT && current->type != APPEND_OUT)
			current = current->next;
		handle_output_redirection(current);			
		while (current->prev && current->type != 6)
			current = current->prev;
		if (is_infile == 0)
			execute(p->token->value, current, p);
		else
			execute(current->value, current, p);
	}
	// return (ft_wait(pid));
	return (0);
}
