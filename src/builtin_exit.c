/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/13 19:09:26 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
exit ferme le shell si:
- il est la seule commande a executer
- s'il a un seul arg meme invalide (si invalide => msg d'erreur)

Message d'erreur:
- si 1 seul arg > long long max ou < long long min => not a numeric arg
- si plusieurs args et que le 1er est valide => too many args et n'exit pas
- si plusieurs args et 1er invalide => not a numeric argument
*/

void	input_ok(t_pipex *p, char *cmd, PARSER *node)
{
	if (p->flag == 1)
	{
		ft_putstr_fd("exit\n", 1);
		p->exit = 1;
	}
	if (cmd)
		node->exit_code = ft_strtoll(cmd) % 256;
}

void	too_many(t_pipex *p, PARSER *node)
{
	if (p->flag == 1)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: too many arguments\n", 2);
	}
	node->exit_code = 1;
}

void	not_a_num(t_pipex *p, PARSER *node)
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

int	check_exit_arg(char *cmd)
{
	int	j;

	j = 0;
	while (cmd[j])
	{
		if (cmd[0] == '-' || cmd[0] == '+')
			j++;
		if (!isdigit(cmd[j]))
			return (FALSE);
		j++;
	}
	return (TRUE);
}

void	free_exit(t_pipex *p, PARSER *nodes, int exit_c)
{
	reset_node(&nodes);
	free_t_env(p->env_nodes);
	free_pipex(&p);
	exit(exit_c);
}

// trop d'arguments
int	ft_exit(char **cmd, t_pipex *p, PARSER *node, int *cpy_stdin, int *cpy_stdout)
{
	int	i;
	int	exit_code;

	i = 1;
	if (cmd[i])
	{
		if (!check_exit_arg(cmd[i])
			|| !lenght_exit_code(cmd[1]))
		{
			not_a_num(p, node);
			restore_std(cpy_stdin, cpy_stdout);
			free_exit(p, node, 2);
		}
		i++;
	}
	if (cmd[i])
		too_many(p, node);
	else
	{
		input_ok(p, cmd[1], node);
		restore_std(cpy_stdin, cpy_stdout);
		exit_code = node->exit_code;
		free_exit(p, node, exit_code);
	}
	return (TRUE);
}
