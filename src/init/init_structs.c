/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_t_pipex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:19:17 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 18:23:11 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipex(t_pipex *p, t_env **chained_env, t_parser *nodes)
{
	p->env_n = chained_env;
	p->nb_cmd = ft_size_list(&nodes);
	p->i = 0;
	p->d = 0;
	p->prev_fd = -1;
	p->pid = 0;
	p->last_pid = 0;
	p->exit = 0;
	p->flag = 0;
	p->pipefd[0] = -1;
	p->pipefd[1] = -1;
}

void	init_mega(t_mega *mini)
{
	mini->p = NULL;
	mini->L_input = NULL;
	mini->nodes = NULL;
	mini->begin = NULL;
	mini->tokens = NULL;
	mini->chained_env = NULL;
	mini->exit_code = 0;
	mini->str = NULL;
	mini->f = 0;
	mini->d = 0;
	mini->cmd = 0;
	mini->idx = 0;
}
