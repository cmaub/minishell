/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:26:15 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 15:28:43 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	s_clse_array(int **array, t_parser **node)
{
	int	i;

	if (!array || !*array)
		return ;
	i = 0;
	while (i < (*node)->nb_heredoc)
	{
		if (array[i])
			s_clse(&array[i][0]);
		i++;
	}
}
void	close_pipefds(t_pipex *p)
{
	if (p->pipefd[0] != -1)
		s_clse(&p->pipefd[0]);
	if (p->pipefd[1] != -1)
		s_clse(&p->pipefd[1]);
	if (p->prev_fd != -1)
		s_clse(&p->prev_fd);
}

int	cpy_std(int *cpy_stdin, int *cpy_stdout)
{
	*cpy_stdin = dup(STDIN_FILENO);
	if (!*cpy_stdin)
	{
		perror("cpy_stdin");
		return (FALSE);
	}
	*cpy_stdout = dup(STDOUT_FILENO);
	if (!*cpy_stdout)
	{
		perror("cpy_stdout");
		return (FALSE);
	}
	return (TRUE);
}

int	restore_std(int *cpy_stdin, int *cpy_stdout)
{
	if (*cpy_stdout != -1)
	{
		if (dup2(*cpy_stdout, STDOUT_FILENO) == -1)
			perror("STDOUT");
		s_clse(cpy_stdout);
		*cpy_stdout = -1;
	}
	if (*cpy_stdin != -1)
	{
		if (dup2(*cpy_stdin, STDIN_FILENO) == -1)
		{
			perror("STDIN");
			return (FALSE);
		}
		s_clse(cpy_stdin);
		*cpy_stdin = -1;
	}
	if (*cpy_stdout != -1)
		return (FALSE);
	return (TRUE);
}
