/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:33:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/13 11:50:41 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Supprime une ou des variable d'environnement
// Gerer la sppression multiple
// Si une variable n'existe pas, unset ne doit pas provoquer la fin du programme
// faut-il permettre la suppression de variable protégees ?

//parcourir env, trouver la variable
// copier une copie de env sans la variable a supprimer
// a priori pas de message d'erreur

#include "minishell.h"

int		count_env_var(char **list)
{
	int	len;

	len = 0;
	while (list[len] != NULL)
	{
		len++;
	}
	return (len);
}

t_env	**ft_unset(PARSER *current, t_env **env_nodes)
{
	int		i;
	int		j;
	int		index;
	int		size_env;
	t_env	*temp;
	t_env	*saved;

	i = 1;
	saved = NULL;
	if (!env_nodes || !*env_nodes)
	{	
		dprintf(2, "env_nodes n'existe pas");
		return (NULL);
	}
	if (!current->command[1])
		return (env_nodes);
	if (current->command[1] && current->command[1][0] == '-')
	{
		ft_putstr_fd("unset: ", 2);
		ft_putstr_fd(current->command[1], 2);
		ft_putendl_fd(": invalid option", 2);
		current->exit_code = 2;
		return (env_nodes);
	}
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
			temp = *env_nodes;
			j = 0;
			if (index == 0)
			{
				*env_nodes = temp->next;
				free(temp->var);
				free(temp);
			}
			else
			{
				while (temp && j < index)
				{
					saved = temp;
					temp = temp->next;
					j++;
				}
				if (!temp)
				{
					return (env_nodes);
				}
				saved->next = temp->next;
				free(temp->var);
				free(temp);
			}
		}
		i++;
	}
	return (env_nodes);
}

