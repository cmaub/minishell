/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:33:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/21 16:30:11 by cmaubert         ###   ########.fr       */
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
	while (list[len])
		len++;
	return (len);
}

char	**suppress_var(char **env, int index)
{
	char	**new_env;
	int		new_size;
	int		i;
	int		j;

	new_size = count_env_var(env) - 1;
	new_env = try_malloc(new_size * sizeof(char *));

	i = 0;
	j = 0;
	while (i <= new_size)
	{
		if (i != index)
		{
			new_env[j] = ft_strdup(env[i]);
			j++;
		}
		else
			free(env[i]);
		i++;
	}
	new_env[j] = NULL;
	free(env);
	return (new_env);
}

char	**ft_unset(PARSER *current, char **env)
{
	int		i;
	int		index;
	char	**new_env;

	i = 1;
	new_env = NULL;
	if (current->command[1] && current->command[1][0] == '-')
	{
		ft_putstr_fd("unset:", 2);
		ft_putstr_fd(current->command[1], 2);
		ft_putendl_fd(": invalid option", 2);
		current->exit_code = 2;
		return (new_env);
	}
	while (current->command[i])
	{
		index = env_var_exists(env, current->command[i]);
		if (index >= 0)
		{
			new_env = suppress_var(env, index);
			env = new_env;
		}
		i++;
	}
	return (new_env);
}

