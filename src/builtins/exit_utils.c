/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:13:20 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 14:13:30 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	input_ok(t_pipex *p, char *cmd, t_parser *node)
{
	if (p->flag == 1)
	{
		ft_putstr_fd("exit\n", 1);
		p->exit = 1;
	}
	if (cmd)
		node->exit_code = ft_strtoll(cmd) % 256;
}

void	too_many(t_pipex *p, t_parser *node)
{
	if (p->flag == 1)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: too many arguments\n", 2);
	}
	node->exit_code = 1;
}

void	not_a_num(t_pipex *p, t_parser *node)
{
	if (p->flag == 1)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: numeric argument required\n", 2);
		p->exit = 1;
	}
	node->exit_code = 2;
}

int	lenght_exit_code(char *cmd)
{
	long long int	max;
	long long int	min;
	char			*max_str;
	char			*min_str;

	max = 9223372036854775807;
	min = -9223372036854775807;
	max_str = "9223372036854775807";
	min_str = "-9223372036854775807";
	if (((ft_strtoll(cmd) == max) || (ft_strtoll(cmd) == min))
		&& ((ft_strncmp(cmd, max_str, ft_strlen(cmd)) != 0)
			&& (ft_strncmp(cmd, min_str, ft_strlen(cmd)) != 0)))
		return (FALSE);
	return (TRUE);
}