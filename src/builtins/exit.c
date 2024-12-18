/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 14:19:22 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
exit closes the shell if:
- it is the only command to execute
- if it has only one arg even invalid (if invalid => error msg)

Error message:
- if only 1 arg > long long max or < long long min => not a numeric arg
- if several args and the 1st is valid => too many args and no exit
- if several args and 1st invalid => not a numeric argument
*/

int	check_exit_arg(char *cmd)
{
	int	j;

	j = 0;
	while (cmd[j])
	{
		if (cmd[0] == '-' || cmd[0] == '+')
			j++;
		if (!isdigit(cmd[j]))
			return (FALSE);
		j++;
	}
	return (TRUE);
}

void	free_exit(t_pipex *p, t_mega *mini, int exit_c)
{
	free_env(p->env_n);
	free_pipex(&p);
	if (mini->begin)
		rst_nde(&mini->begin);
	if (mini)
		free(mini);
	exit(exit_c);
}

void	invalid_arg(t_pipex *p, t_parser *node, t_cpy *cpy, t_mega *mini)
{
	not_a_num(p, node);
	if (cpy)
		restore_std(&cpy->cpy_stdin, &cpy->cpy_stdout);
	free_exit(p, mini, 2);
}

void	valid_exit(t_pipex *p, t_parser *node, t_cpy *cpy, t_mega *mini)
{
	int	exit_code;

	if (cpy)
		restore_std(&cpy->cpy_stdin, &cpy->cpy_stdout);
	exit_code = node->exit_code;
	free_exit(p, mini, exit_code);
}

int	ft_exit(t_pipex *p, t_parser *node, t_cpy *cpy, t_mega *mini)
{
	if (node->command[1])
	{
		if (!check_exit_arg(node->command[1])
			|| !lenght_exit_code(node->command[1]))
			invalid_arg(p, node, cpy, mini);
	}
	if (node->command[2])
		too_many(p, node);
	else
	{
		input_ok(p, node->command[1], node);
		valid_exit(p, node, cpy, mini);
	}
	return (TRUE);
}

