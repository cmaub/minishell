/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:33:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/05 15:17:20 by anvander         ###   ########.fr       */
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
		// dprintf(2, "list[len] dans count env var = %s\n", list[len]);
		len++;
	}
	return (len);
}

char	**suppress_var(char **env, int index)
{
	char	**new_env;
	int		new_size;
	int		i;
	int		j;

	new_size = count_env_var(env) - 1;
	dprintf(2, "new_size = %d\n", new_size);
	new_env = try_malloc((new_size + 1)* sizeof(char *));
	if (!new_env)
	{
		return (NULL);
	}
	i = 0;
	j = 0;
	while (i <= new_size)
	{
		if (i != index)
		{
			new_env[j] = ft_strdup(env[i]);
			free(env[i]);
			// env[i] = NULL;
			j++;
		}
		else
		{
			dprintf(2, "env[%d]= %s\n", i, env[i]);
			free(env[i]);
			env[i] = NULL;
		}
		i++;
	}
	new_env[j] = NULL;
	dprintf(2, "size de new_env apres alloc = %d\n", count_env_var(new_env));
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
	if (!current->command[1])
		return (env);
	
	if (current->command[1] && current->command[1][0] == '-')
	{
		ft_putstr_fd("unset: ", 2);
		ft_putstr_fd(current->command[1], 2);
		ft_putendl_fd(": invalid option", 2);
		current->exit_code = 2;
		return (env);
	}
	while (current->command[i])
	{
		index = env_var_exists(env, current->command[i]);
		dprintf(2, "index = %d\n", index);
		if (index >= 0)
		{
			dprintf(2, "i = %d\n", i);
			// dprintf(2, "nb de var d'env = %d\n", count_env_var(env));
			new_env = suppress_var(env, index);
			// dprintf(2, "new_env = %s (%s, %d)\n", new_env[0], __FILE__, __LINE__);
			if (!new_env)
			{
				current->exit_code = 1;
				write(1, "fatal: out of memory\n", 21);
				return (env);
			}
			env = new_env;
			// dprintf(2, "env = %s (%s, %d)\n", env[0], __FILE__, __LINE__);
			// free(new_env);
			// dprintf(2, "env = %s (%s, %d)\n", env[0], __FILE__, __LINE__);
			i++;
		}
		else
			i++;
	}
	// if (!new_env)
	// 	return (env);
	// dprintf(2, "env = %s (%s, %d)\n", env[0], __FILE__, __LINE__);
	return (env);
}

