/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/11 18:09:26 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		all_n(char *str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE);
}


int	ft_echo(char **cmd)
{
	int	i;
	int	is_n;

	i = 1;
	is_n = 0;
	if (!cmd[i])
		return (TRUE);
	while (ft_strncmp(cmd[i], "-n", 2) == 0 && all_n(cmd[i]))
	{
		i++;
		is_n = 1;
	}
	if (!cmd[i])
		return (TRUE);
	if (cmd[i][0] != '-' || !all_n(cmd[i]))
	{
		while (cmd[i])
		{
			if (ft_putstr_fd(cmd[i], 1) == -1)
				return (FALSE);
			if (cmd[i + 1])
				write(1, " ", 1);
			i++;
		}
		if (is_n == 0)
			write(1, "\n", 1);
		return (TRUE);
	}
	return (TRUE);
}

	
