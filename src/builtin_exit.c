/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/21 10:56:26 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
exit ferme le shell si:
- il est la seule commande a executer
- s'il a un seul argument meme invalide (si invalide il enverra un msg d'erreur)

Message d'erreur:
- si 1 seul argument > long long max ou < long long min => not a numeric argument
- si plusieurs arguments et que le premier est valide => too many arguments et n'ext pas
- si plusieurs arguments et premier invalide => not a numeric argument
*/

void	input_ok(t_pipex *p, char *cmd, PARSER *node)
{
	if (p->flag == 1)
	{
		ft_putstr_fd("exit\n", 2);
		p->exit = 1;
	}
	if (cmd)
		node->exit_code = ft_strtoll(cmd) % 256;
}

void	too_many(t_pipex *p, PARSER *node)
{
	if (p->flag == 1)
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("exit: too many arguments\n", 2);
	}
	node->exit_code = 1;
}

void	not_a_num(t_pipex *p, PARSER *node)
{
	if (p->flag == 1)
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("exit: not a numeric argument\n", 2);
		p->exit = 1;
	}
	node->exit_code = 2;
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

void	check_exit_arg(char *cmd, PARSER *node, t_pipex *p)
{
	int	j;

	j = 0;
	while (cmd[j])
	{
		if (cmd[0] == '-')
			j++;
		if (!isdigit(cmd[j]))
		{
			not_a_num(p, node);
			exit (node->exit_code);
		}
		j++;
	}
}

int	ft_exit(char **cmd, t_pipex *p, PARSER *node)
{
	int	i;

	i = 1;
	if (cmd[i])
	{
		check_exit_arg(cmd[i], node, p);
		if (is_arg_too_big(cmd[1]))
		{
			not_a_num(p, node);
			exit(node->exit_code);
		}
		i++;
	}
	if (cmd[i])
		too_many(p, node);
	else
	{
		input_ok(p, cmd[1], node);
		exit(node->exit_code);
	}
	return (TRUE);
}
