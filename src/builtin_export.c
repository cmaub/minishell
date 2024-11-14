/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:04:35 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/14 19:43:33 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export rend une variable d'environnement accessible aux processus enfants.
// il permet de rajouter une ou plusieurs variables d'environnement, en les initialisant ou non
// il faut donc verifier si elles existent deja avant de les creer
// si elles existent deja, il est possible de changer la valeur de la variable d'env deja existante
// si export sans arg on affiche les variables d'environnement dans l'ordre ascii
// verifier la syntaxe des noms de variables + des valeurs possibles

char	**expand_env(char **env, int old_size) // ne fonctionne pas
{
	char	**new_env;
	int	i;

	new_env = ft_calloc(old_size + 2, sizeof(char *));
	i = 0;
	if (!new_env)
		return (NULL);
	while (env[i] && i < old_size)
	{
		new_env[i] = env[i];
		i++;
	}
	free(env);
	return (new_env);
}

int		env_var_exists(char **env, char *var)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, strlen(var)) == 0 && env[i][strlen(var)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void	sort_tab_ascii(char **env, int count)
{
	int	i;
	int	j;
	int	min_idx;
	char *temp;

	i = 0;
	temp = NULL;
	while (i < count)
	{
		min_idx = i;
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(env[j], env[min_idx]) < 0)
				min_idx = j;
			j++;
		}
		if (min_idx != i)
		{
			temp = env[i];
			env[i] = env[min_idx];
			env[min_idx] = temp;
		}
		i++;
	}
}

void	print_sorted_env(char **env)
{
	int		count;
	int		i;
	char	**sorted_env;

	count = 0;
	i = 0;
	sorted_env = NULL;
	while (env[count])
		count++;
	sorted_env = ft_calloc(count + 1, sizeof(char *));
	if (!sorted_env)
		return ; // revoir si valeur de retour
	while (i < count)
	{
		sorted_env[i] = env[i];
		i++;
	}
	sorted_env[count] = NULL;
	sort_tab_ascii(sorted_env, count);
	i = 0;
	while (sorted_env[i])
	{
		ft_putstr_fd(sorted_env[i], 1);
		write(1, "\n", 1);
		i++;
	}
	free(sorted_env);
}

int	check_name(char *name)
{
	int	i;

	i = 0;
	if (!name || !name[0] || ft_isdigit(name[0]))
		return (FALSE);
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	ft_export(char **cmd, char **env)
{
	int		i;
	int		j;
	int		index;
	char	*equal;
	char	*name;
	char	*value;

	i = 1;
	j = 0;
	equal = NULL;
	if (!cmd[1]) // si pas d'arg afficher par ordre ascii
	{
		print_sorted_env(env);
		return (TRUE);
	}
	while (cmd[i] != NULL)
	{
		// on separe le nom et la valeur (ex :  TEST=value --> "TEST" et "value")
		equal = ft_strchr(cmd[i], '=');
		// printf("%d - equal = %s\n", i, equal);
		if (equal) // si initialisation d'une variable
		{
			equal[0] = '\0';
			name = cmd[i];
			value = equal + 1;
			printf("value = %s\n", value);
			printf("name = %s\n", name);
			if (!check_name(name))
			{
				ft_putstr_fd("minishell: export: '", 2);
				ft_putstr_fd(name, 2);
				ft_putstr_fd("': not a valid identifier\n", 2);

				i++;
				continue;
			} //message d'erreur ?
			index = env_var_exists(env, name);
			*equal = '=';
			printf("env[%d] = %s\n", index, env[index]);
			// printf("cmd[%d] = %s\n", i, cmd[i]);
			if (index >= 0)
			{
				free(env[index]);
				env[index] = ft_strdup(cmd[i]);
				printf("env[%d] = %s\n", index, env[index]);
			}
			else
			{
				while (env[j])
					j++;
				// env = expand_env(env, j); // Agrandir env si besoin
				// env[j] = ft_strdup(cmd[i]);
				// env[j + 1] = NULL;
				// printf("LINE = %d\n", __LINE__);
				env[j] = ft_strdup(cmd[i]);
				env[j + 1] = NULL;
				printf("env[%d] = %s\n", j, env[j]);
			}
		}
		else // ajouter variable sans valeur dans env
		{
			if (!check_name(cmd[i]))
			{
				printf("cmd[%d] = %s\n", i, cmd[i]);
				printf("LINE = %d\n", __LINE__);
				ft_putstr_fd("minishell: export: '", 2);
				ft_putstr_fd(cmd[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				i++;
				continue;
			}
			index = env_var_exists(env, cmd[i]);
			if (index < 0) // Si la variable n'existe pas, l'ajouter avec une valeur vide
			{
				j = 0;
				while (env[j])
					j++;
				env[j] = ft_strdup(cmd[i]);
				env[j + 1] = NULL;
				printf("env[%d] = %s\n", j, env[j]);
			}
		}
		i++;
		// printf("LINE = %d\n", __LINE__);
	}
	printf("LINE = %d\n", __LINE__);
	return (TRUE);
}

