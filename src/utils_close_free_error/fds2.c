/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:05:13 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/19 09:52:23 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredoc(t_parser *current)
{
	int	i;

	i = 0;
	while (i < current->nb_heredoc)
	{
		s_clse(&current->fd_heredoc[i][0]);
		s_clse(&current->fd_heredoc[i][1]);
		i++;
	}
}

void	close_all_heredoc(t_mega *mini)
{
	t_parser	*current;

	if (!mini || !mini->nodes)
		return ;
	current = mini->begin;
	while (current)
	{
		close_heredoc(current);
		current = current->next;
	}
}

void	clse_n_x(int *fd, t_pipex *p, char *str)
{
	if (fd)
		s_clse(fd);
	s_clse(&p->pipefd[1]);
	s_clse(&p->pipefd[0]);
	if (str)
		perror(str);
}
