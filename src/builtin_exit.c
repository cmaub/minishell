/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/13 15:59:58 by anvander         ###   ########.fr       */
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

void	input_ok(t_pipex *p, char *cmd)
{
	ft_putstr_fd("exit\n", 2);
	p->exit = 1;
	if (cmd)
		p->exit_status = ft_strtoll(cmd) % 256;
}

int	too_many(t_pipex *p)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("exit: too many arguments\n", 2);
	p->exit_status = 1;
	return (TRUE);
}

int	not_a_num(t_pipex *p)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("exit: not a numeric argument\n", 2);
	p->exit = 1;
	p->exit_status = 2;
	return (TRUE);
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
				return (not_a_num(p));
			j++;
		}
		// ne suffit pas pour le LONG LONG MIN
		if (is_arg_too_big(cmd[1]))
			return (not_a_num(p));
		i++;
	}
	if (cmd[i])
		return (too_many(p));
	else
		input_ok(p, cmd[1]);
	return (TRUE);
}
