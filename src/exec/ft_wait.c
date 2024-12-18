/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:19:20 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 18:35:01 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_exit_code(int status, t_parser **nodes, int *status_code)
{
	if (WIFEXITED(status))
	{
		*status_code = WEXITSTATUS(status);
		(*nodes)->exit_code = *status_code;
	}
	else if (g_signal != 0)
	{
		*status_code = 128 + g_signal;
		(*nodes)->exit_code = *status_code;
		g_signal = 0;
	}
}

int	ft_wait(pid_t last_pid, t_parser **nodes)
{
	int			status;
	int			status_code;
	pid_t		waited_pid;
	t_parser	*current;	

	status_code = 0;
	current = *nodes;
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
			update_exit_code(status, nodes, &status_code);
		waited_pid = wait(&status);
	}		
	return (signal(SIGINT, handle_c_signal), status_code);
}
