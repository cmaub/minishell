/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:33:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/18 14:59:26 by cmaubert         ###   ########.fr       */
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

// void	print_env(char **env)
// {
// 	int	i;

// 	i = 0;
// 	while (env[i])
// 	{
// 		printf("env = %s\n", env[i]);
// 		// ft_putstr_fd(env[i], 1);
// 		// write(1, "\n", 1);
// 		i++;
// 	}
// }

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
	new_env = ft_calloc(new_size + 1, sizeof(char *));
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

char	**ft_unset(char **cmd, char **env)
{
	int		i;
	int		index;
	char	**new_env;

	i = 1;
	// printf("LINE = %d\n", __LINE__);
	// print_env(env);
	printf("before = count_env_var(env) = %d\n", count_env_var(env));
	while (cmd[i])
	{
		index = env_var_exists(env, cmd[i]);
		dprintf(2, "index = %d\n", index);
		if (index >= 0)
		{
			// printf("index = %d\n", index);
			new_env = suppress_var(env, index);
			if (!new_env)
				return (FALSE);
			// print_env(new_env);
			env = new_env;
			printf("after = count_env_var(env) = %d\n", count_env_var(env));
			// print_sorted_env(new_env);
			// print_env(env);
		}
		i++;
	}
	return (new_env);
}

// int		main(int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	char		**mini_env;

// 	mini_env = copy_env(env);
// 	if (ft_strncmp(argv[1], "unset", 5) == 0)
// 		{
// 			ft_export(argv + 1, mini_env);
// 			// print_sorted_env(mini_env);
// 		}
// 	return (0);
// }
