/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:19:20 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/19 10:23:57 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_exit_code(int status, t_parser **nodes, int *code, int *flag)
{
	if (WIFEXITED(status))
	{
		*code = WEXITSTATUS(status);
		(*nodes)->exit_code = *code;
		*flag = 1;
	}
	else if (g_signal != 0)
	{
		*code = 128 + g_signal;
		(*nodes)->exit_code = *code;
		g_signal = 0;
	}
}

int	ft_wait(pid_t last_pid, t_parser **nodes)
{
	int			status;
	int			status_code;
	pid_t		waited_pid;
	t_parser	*current;
	int			flag;

	status_code = 0;
	current = *nodes;
	flag = 0;
	if (current == NULL)
		return (0);
	waited_pid = wait(&status);
	while (waited_pid != -1)
	{
		if (current && current->next)
			current = current->next;
		else
			current = *nodes;
		if (waited_pid == last_pid)
			update_exit_code(status, nodes, &status_code, &flag);
		waited_pid = wait(&status);
	}		
	if (flag == 1)
		g_signal = 0;
	return (status_code);
}
