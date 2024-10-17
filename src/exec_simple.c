/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:34:09 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/17 15:34:09 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	simple_cmd(t_pipex *p)
{
	pid_t	pid;
	int		exit_code;
	int		fd_in;
	int		fd_out;
	int		nb_of_tokens;
	int		is_file;
	t_token	*current;

	is_file = 0;
	nb_of_tokens = list_size(p->token);
	pid = fork();
	if (pid == - 1)
		ft_error("fork");
	else if (pid == 0)
	{
		if (p->token->type == REDIRECT_IN)
		{
			is_file = 1;
			fd_in = open(p->token->next->value, O_RDONLY | 0644);
			if (fd_in == -1)
				ft_error("open");
			if (dup2(fd_in, STDIN_FILENO) == -1)
				ft_error("dup2");
			close(fd_in);			
		}
		/* ATTENTION AU HEREDOC*/
		while (current->index != nb_of_tokens - 1)
			current = current->next;
		if (current->type == REDIRECT_OUT)
		{
			fd_out = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_out == -1)
				ft_error("open");
			if (dup2(fd_out, STDOUT_FILENO) == -1)
				ft_error("dup2");
			close(fd_out);
		}
		else if (current->type == APPEND_OUT)
		{
			fd_out = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd_out == -1)
				ft_error("open");
			if (dup2(fd_out, STDOUT_FILENO) == -1)
				ft_error("dup2");
			close(fd_out);
		}
		if (is_file == 0)
		{
			if (ft_execve(token->value, envp) == -1)
				ft_error("exec");
		}
		else
		{
			if (ft_execve(current->value, envp) == -1)
				ft_error("exec");
		}
		
	}
	return (ft_wait(last_pid));
}