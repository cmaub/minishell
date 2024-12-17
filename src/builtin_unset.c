/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:33:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/17 09:41:15 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Supprime une ou des variable d'environnement
// Si une variable n'existe pas, unset ne doit pas provoquer la fin du programme

#include "minishell.h"

int	count_env_var(char **list)
{
	int	len;

	len = 0;
	while (list[len] != NULL)
	{
		len++;
	}
	return (len);
}

int	check_args_unset(PARSER *current, t_env **env_nodes)
{
	if (!env_nodes || !*env_nodes)
	{
		env_nodes = NULL;
		return (FALSE);
	}
	if (!current->command[1])
		return (FALSE);
	if (current->command[1] && current->command[1][0] == '-')
	{
		(ft_putstr_fd("unset: ", 2), ft_putstr_fd(current->command[1], 2),
			ft_putendl_fd(": invalid option", 2));
		current->exit_code = 2;
		return (FALSE);
	}
	return (TRUE);
}

t_env	**browse_env_and_unset_var(t_env **env_nodes, int index)
{
    int        j;
    t_env    *temp;
    t_env    *saved;

	j = -1;
	temp = *env_nodes;
	saved = NULL;
	if (index == 0)
	{
		*env_nodes = temp->next;
		(free(temp->var), free(temp));
	}
	else
	{
		while (temp && j++ < index -1)
		{
			saved = temp;
			temp = temp->next;
		}
		if (!temp)
			return (env_nodes);
		saved->next = temp->next;
		(free(temp->var), free(temp));
	}
	return (env_nodes);
}

t_env    **ft_unset(PARSER *current, t_env **env_nodes)
{
    int        i;
    int        index;
    int        size_env;

	i = 1;
	if (!check_args_unset(current, env_nodes))
		return (env_nodes);
	while (current->command[i] != NULL)
	{
		index = env_var_exists(env_nodes, current->command[i]);
		if (index >= 0)
		{
			size_env = lstsize_t_env(env_nodes);
			if (size_env == 1)
			{
				ft_putendl_fd("empty envp not allowed\n", 2);
				break ;
			}
			env_nodes = browse_env_and_unset_var(env_nodes, index);
		}
		i++;
	}
	return (env_nodes);
}
