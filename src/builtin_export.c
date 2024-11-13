/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:04:35 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/13 22:56:02 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export rend une variable d'environnement accessible aux processus enfants.
// il permet de rajouter une ou plusieurs variables d'environnement, en les initialisant ou non
// il faut donc verifier si elles existent deja avant de les creer
// si elles existent deja, il est possible de changer la valeur de la variable d'env deja existante
// si export sans arg on affiche les variables d'environnement dans l'ordre ascii
// verifier la syntaxe des noms de variables + des valeurs possibles

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

void	tab_sort_alpha(char **env, int count)
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
			if (ft_strncmp(env[j], env[min_idx] < 0))//revoir
				min_idx = j;
			j++;
		}
		if (min_idx != i)
		{
			temp = env[i];
			env[i] = env[min_idx];
			env[min_idx] = temp;
		}
	}
}

void	print_sorted_env(char **env)
{
	int	count;
	int i;
	char **sorted_env;

	count = 0;
	i = 0;
	while (env[count])
		count++;
	sorted_env = malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
		return ; // revoir si valeur de retour
	while (i < count)
	{
		sorted_env[i] = env[i];
		i++;
	}
	sorted_env[count] = NULL;

	tab_sort_alpha(sorted_env, count);

	i = 0;
	while (sorted_env[i])
	{
		ft_putstr_fd(sorted_env[i], 1);
		write(1, "\n", 1);
		i++;
	}
	free(sorted_env);
}

int	is_valid_name(char *name)
{
	int i;

	i = 0;
	if (!name || ft_isdigit(name[0]))
		return (FALSE);
	while (name[i])
	{
		if (!ft_isalnum(name[i]))
			return (FALSE);
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
	equal = NULL;
	if (!cmd[1]) // si pas d'arg
	{
		print_sorted_env(env);
		return (TRUE);
	}
	while (cmd[i])
	{
		//on separe le nom et la valeur
		equal = ft_strchr(cmd[i], '=');
		if (equal) // si initialisation d'une variable
		{
			equal = '\0';
			name = cmd[i];
			value = equal + 1;
			if (!is_valid_name(name))
				return (FALSE); //message d'erreur ?
			index = env_var_exists(env, name);
			if (index >= 0)
			{
				free(env[index]);
				env[index] = ft_strdup(cmd[i]);
			}
			else
			{
				j = 0;
				while (env[j])
					j++;
				env[j] = ft_strdup(cmd[i]);
				env[j + 1] = '\0';
			}
		}
		else // ajouter variable sans valeur dans env
		{
			if (!is_valid_name(name))
				return (FALSE);
			j = 0;
			while (env[j])
				j++;
			if (!env[j])
				return (FALSE);
			env[j] = ft_strdup(name);
			env[j + 1] = '\0';
		}
		i++;
	}
	return (TRUE);
}

