/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:25:33 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 17:21:38 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_redirection(t_parser **n, t_pipex *p, int *d)
{
	int	fd_in;

	fd_in = -1;
	if ((*n)->redir[(*n)->f] == REDIRECT_IN)
			fd_in = open((*n)->file[(*n)->f], O_RDONLY | 0644);
	if (fd_in == -1 && (*n)->redir[(*n)->f] == REDIRECT_IN)
		return (clse_n_x(NULL, p, (*n)->file[(*n)->f]), FALSE);
	if ((*n)->redir[(*n)->f] == REDIRECT_IN)
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
			return (clse_n_x(&fd_in, p, (*n)->file[(*n)->f]), FALSE);
		s_clse(&fd_in);
	}
	if ((*n)->redir[(*n)->f] == HEREDOC)
	{
		s_clse(&(*n)->fd_heredoc[*d][1]);
		if (dup2((*n)->fd_heredoc[*d][0], STDIN_FILENO) == -1)
		{
			clse_n_x(&(*n)->fd_heredoc[*d][0], p, (*n)->file[(*n)->f]);
			return (TRUE);
		}
		s_clse(&(*n)->fd_heredoc[*d][0]);
		*d += 1;
	}
	return (TRUE);
}

int	handle_output_redirection(t_parser **n, t_pipex *p, int *flag_output)
{
	int	fd_out;

	fd_out = -1;
	if ((*n)->redir[(*n)->f] != REDIRECT_OUT
		&& (*n)->redir[(*n)->f] != APPEND_OUT)
		return (TRUE);
	if (flag_output)
		*flag_output = 1;
	if ((*n)->redir[(*n)->f] == REDIRECT_OUT)
		fd_out = open((*n)->file[(*n)->f], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if ((*n)->redir[(*n)->f] == APPEND_OUT)
		fd_out = open((*n)->file[(*n)->f], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		clse_n_x(NULL, p, (*n)->file[(*n)->f]);
		return (FALSE);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		clse_n_x(&fd_out, p, (*n)->file[(*n)->f]);
		return (FALSE);
	}
	s_clse(&fd_out);
	return (TRUE);
}
