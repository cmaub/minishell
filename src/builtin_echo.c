/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/12 15:59:13 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **cmd)
{
	int	i;

	i = 0;
	if (ft_strncmp(cmd[1], "-n", 2) == 0)
	{
		i = 2;
		while (cmd[i])
		{
			ft_putstr_fd(cmd[i], 1);
			if (cmd[i + 1])
				write(1, " ", 1);
			i++;
		}
	}
	else
	{
		i = 1;
		while (cmd[i])
		{
			ft_putstr_fd(cmd[i], 1);
			if (cmd[i + 1])
				write(1, " ", 1);
			i++;
		}
		write(1, "\n", 1);
	}
	return (TRUE);
}
