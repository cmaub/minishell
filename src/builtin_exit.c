/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/13 10:33:56 by anvander         ###   ########.fr       */
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
			if (!ft_isdigit(cmd[i][j]))
			{
				ft_putstr_fd("exit\n", 2);
				ft_putstr_fd("exit: not a numeric argument\n", 2);
				p->exit = 1;
				return (TRUE);
			}
			j++;
		}
		if ((ft_atoi(cmd[i]) > 9223372036854775807)
			|| (ft_atoi(cmd[i]) < -9223372036854775807))
		{
			ft_putstr_fd("exit\n", 2);
			ft_putstr_fd("exit: not a numeric argument\n", 2);
			p->exit = 1;
			return (TRUE);
		}
		i++;
	}
	if (cmd[i])
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (TRUE);
	}
	else
	{
		ft_putstr_fd("exit\n", 2);
		p->exit = 1;
	}
	return (TRUE);
}
