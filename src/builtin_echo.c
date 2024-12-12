/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/12 11:58:04 by anvander         ###   ########.fr       */
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

int	check_args(char **cmd, int *i, int *is_n)
{
	if (!cmd[*i])
		return (-1);
	while (ft_strncmp(cmd[*i], "-n", 2) == 0 && all_n(cmd[*i]))
	{
		(*i)++;
		*is_n = 1;
	}
	if (!cmd[*i])
		return (-1);
	else
		return (0);
}

int	ft_echo(char **cmd)
{
	int	i;
	int	is_n;

	i = 1;
	is_n = 0;
	if (check_args(cmd, &i, &is_n) < 0)
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

	
