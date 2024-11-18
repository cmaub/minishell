/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:32:16 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/18 13:44:14 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **cmd, char **mini_env)
{
	if (cmd[1])
	{
		ft_putstr_fd("Too many arguments\n", 2);
		exit (EXIT_FAILURE);
	}
	dprintf(2, "entree dans la fonction\n");
	while (*mini_env)
	{
		dprintf(2, "*mini_env = %s\n", *mini_env);
		ft_putstr_fd(*mini_env, 1);
		write(1, "\n", 1);
		mini_env++;
	}
	return (TRUE);
}
