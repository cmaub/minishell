/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:29:58 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/19 10:23:25 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_c_signal_heredoc(int signum)
{
	g_signal = signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_done = 1;
	close(STDIN_FILENO);
}

void	handle_c_signal(int signum)
{
	g_signal = signum;
	ft_putstr_fd("\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	c_child(int signum)
{
	g_signal = signum;
	ft_putstr_fd("\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	q_child(int signum)
{
	g_signal = signum;
	rl_on_new_line();
	rl_replace_line("", 0);
}
