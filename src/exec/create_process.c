/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:36:53 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 15:41:41 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipes(t_pipex *p, t_parser *node)
{
	if (p->nb_cmd > 1 && p->i < p->nb_cmd - 1)
	{
		if (pipe(p->pipefd) == -1)
		{
			perror("pipe");
			node->exit_code = 1;
			return (FALSE);
		}
	}
	return (TRUE);
}

int	create_process(t_parser **nodes, t_pipex *p, t_mega *mini)
{
	t_parser		*current;

	current = (*nodes);
	if (current && current->next == NULL && is_builtin(current))
		return (handle_builtin(current, p, mini));
	while (p->i < p->nb_cmd)
	{
		if (!create_pipes(p, current))
			return (FALSE);
		(signal(SIGINT, SIG_IGN), signal(SIGINT, c_child));
		signal(SIGQUIT, q_child);
		p->pid = fork();
		if (p->pid == -1)
		{
			(*nodes)->exit_code = 1;
			return (perror("fork"), close_pipefds(p), FALSE);
		}
		if (p->i == p->nb_cmd - 1)
			p->last_pid = p->pid;
		choose_process(p, &current, mini);
		p->i++;
		current = current->next;
	}
	return (close_pipefds(p), ft_wait(p->last_pid, nodes));
}