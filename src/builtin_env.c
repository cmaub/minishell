/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:32:16 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/19 17:13:07 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(PARSER *current, char **mini_env)
{
	if (current->command[1])
	{
		ft_putstr_fd("Too many arguments\n", 2);
		current->exit_code = 2;
		return (FALSE);
	}
	while (*mini_env)
	{
		ft_putstr_fd(*mini_env, 1);
		write(1, "\n", 1);
		mini_env++;
	}
	return (TRUE);
}
