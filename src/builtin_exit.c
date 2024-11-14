/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/14 14:52:08 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
exit ferme le terminal si:
- il est la seule commande a executer
- s'il a un seul argument meme invalide (si invalide il enverra un msg d'erreur)

Message d'erreur:
- si 1 seul argument > long long max ou < long long min => not a numeric argument
- si plusieurs arguments et que le premier est valide => too many arguments
- si plusieurs arguments et premier invalide => not a numeric argument
*/

extern int	exit_code;

void	input_ok(t_pipex *p, char *cmd)
{
	ft_putstr_fd("exit\n", 2);
	p->exit = 1;
	if (cmd)
		exit_code = ft_strtoll(cmd) % 256;
}

void	too_many()
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("exit: too many arguments\n", 2);
	exit_code = 1;
}

void	not_a_num(t_pipex *p)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("exit: not a numeric argument\n", 2);
	p->exit = 1;
	exit_code = 2;
}

int	is_arg_too_big(char *cmd)
{
	if (((ft_strtoll(cmd) == 9223372036854775807)
		|| (ft_strtoll(cmd) == -9223372036854775807))
		&& ((ft_strncmp(cmd, "9223372036854775807", ft_strlen(cmd)) != 0) 
		&& (ft_strncmp(cmd, "-9223372036854775807", ft_strlen(cmd)) != 0)))
		return (1);
	else 
		return (0);
}

int	ft_exit(char **cmd, t_pipex *p)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (cmd[i])
	{
		while (cmd[i][j])
		{
			if (cmd[i][0] == '-')
				j++;
			if (!isdigit(cmd[i][j]))
			{
				not_a_num(p);
				exit (exit_code);
			}
			j++;
		}
		// ne suffit pas pour le LONG LONG MIN
		if (is_arg_too_big(cmd[1]))
		{
			not_a_num(p);
			exit(exit_code);
		}
		i++;
	}
	if (cmd[i])
	{
		too_many();
	}
	else
	{
		input_ok(p, cmd[1]);
		exit(exit_code);
	}
	return (TRUE);
}
