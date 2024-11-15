/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:04:35 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/15 14:31:15 by cmaubert         ###   ########.fr       */
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

void	handle_value(char *cmd, char **env)
{
	int		index;
	char	*equal;
	char	*name;
	char	*value;
	int		j;

	equal = ft_strchr(cmd, '=');
	*equal = '\0';
	name = cmd;
	value = equal + 1;
	j = 0;
	// printf("value = %s\n", value);
	// printf("name = %s\n", name);
	if (!check_name(name))
	{
		ft_putstr_fd("minishell: export: '", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		*equal = '=';
		return ;
	} //message d'erreur ?
	index = env_var_exists(env, name);
	*equal = '=';
	printf("env[%d] = %s\n", index, env[index]);
	// printf("cmd[%d] = %s\n", i, cmd[i]);
	if (index >= 0)
	{
		free(env[index]);
		env[index] = ft_strdup(cmd);
		printf("env[%d] = %s\n", index, env[index]);
	}
	else
	{
		while (env[j])
			j++;
		env[j] = ft_strdup(cmd);
		env[j + 1] = NULL;
		printf("env[%d] = %s\n", j, env[j]);
		// add_to_env(env, cmd);
	}

}

void	handle_variable_without_value(char *cmd, char **env)
{
	int index;
	int j;

	j = 0;
	if (!check_name(cmd))
	{
		ft_putstr_fd("minishell: export: '", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return ;
	}
	index = env_var_exists(env, cmd);
	if (index < 0)
	{
		while (env[j])
			j++;
		env[j] = ft_strdup(cmd);
		env[j + 1] = NULL;
		printf("env[%d] = %s\n", j, env[j]);
		// add_to_env(&env, cmd);
	}
}

int	ft_export(char **cmd, char **env)
{
	int	i;

	i = 1;
	if (!cmd[1]) // si pas d'arg afficher par ordre ascii
	{
		print_sorted_env(env);
		return (TRUE);
	}
	while (cmd[i])
	{
		if (ft_strchr(cmd[i], '=') != NULL)
			handle_value(cmd[i], env);
		else
			handle_variable_without_value(cmd[i], env);
		i++;
	}
	return (TRUE);
}


// int		main(int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	char		**mini_env;

// 	mini_env = copy_env(env);
// 	if (ft_strncmp(argv[1], "export", 6) == 0)
// 		{
// 			ft_export(argv + 1, mini_env);
// 			print_sorted_env(mini_env);
// 		}
// 	return (0);
// }

// int	ft_export(char **cmd, char **env)
// {
// 	int		i;
// 	int		j;
// 	int		index;
// 	char	*equal;
// 	char	*name;
// 	char	*value;

// 	i = 1;
// 	j = 0;
// 	equal = NULL;
// 	if (!cmd[1]) // si pas d'arg afficher par ordre ascii
// 	{
// 		print_sorted_env(env);
// 		return (TRUE);
// 	}
// 	while (cmd[i] != NULL)
// 	{
// 		// on separe le nom et la valeur (ex :  TEST=value --> "TEST" et "value")
// 		equal = ft_strchr(cmd[i], '=');
// 		// printf("%d - equal = %s\n", i, equal);
// 		if (equal) // si initialisation d'une variable
// 		{
// 			equal[0] = '\0';
// 			name = cmd[i];
// 			value = equal + 1;
// 			printf("value = %s\n", value);
// 			printf("name = %s\n", name);
// 			if (!check_name(name))
// 			{
// 				ft_putstr_fd("minishell: export: '", 2);
// 				ft_putstr_fd(name, 2);
// 				ft_putstr_fd("': not a valid identifier\n", 2);
// 				*equal = '=';
// 				i++;
// 				continue;
// 			} //message d'erreur ?
// 			index = env_var_exists(env, name);
// 			*equal = '=';
// 			printf("env[%d] = %s\n", index, env[index]);
// 			// printf("cmd[%d] = %s\n", i, cmd[i]);
// 			if (index >= 0)
// 			{
// 				free(env[index]);
// 				env[index] = ft_strdup(cmd[i]);
// 				printf("env[%d] = %s\n", index, env[index]);
// 			}
// 			else
// 			{
// 				while (env[j])
// 					j++;
// 				// env = expand_env(env, j); // Agrandir env si besoin
// 				// env[j] = ft_strdup(cmd[i]);
// 				// env[j + 1] = NULL;
// 				// printf("LINE = %d\n", __LINE__);
// 				env[j] = ft_strdup(cmd[i]);
// 				env[j + 1] = NULL;
// 				printf("env[%d] = %s\n", j, env[j]);
// 			}
// 		}
// 		else // ajouter variable sans valeur dans env
// 		{
// 			if (!check_name(cmd[i]))
// 			{
// 				printf("cmd[%d] = %s\n", i, cmd[i]);
// 				ft_putstr_fd("minishell: export: '", 2);
// 				ft_putstr_fd(cmd[i], 2);
// 				ft_putstr_fd("': not a valid identifier\n", 2);
// 				i++;
// 				continue;
// 			}
// 			index = env_var_exists(env, cmd[i]);
// 			if (index < 0) // Si la variable n'existe pas, l'ajouter avec une valeur vide
// 			{
// 				j = 0;
// 				while (env[j])
// 					j++;
// 				env[j] = ft_strdup(cmd[i]);
// 				env[j + 1] = NULL;
// 				printf("env[%d] = %s\n", j, env[j]);
// 			}
// 		}
// 		i++;
// 		// printf("LINE = %d\n", __LINE__);
// 	}
// 	return (TRUE);
// }

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

// char	**expand_env(char **env, int old_size) // ne fonctionne pas
// {
// 	char	**new_env;
// 	int	i;

// 	new_env = ft_calloc(old_size + 2, sizeof(char *));
// 	i = 0;
// 	if (!new_env)
// 		return (NULL);
// 	while (env[i] && i < old_size)
// 	{
// 		new_env[i] = env[i];
// 		i++;
// 	}
// 	free(env);
// 	return (new_env);
// }

// void	add_to_env(char ***env, char *new_var)
// {
// 	int	 size;
// 	char	**new_env;
// 	int		i;

// 	size = count_env_var(*env);
// 	new_env = ft_calloc(size + 2, sizeof(char *)); // +1 pour la nouvelle var, +1 pour NULL
// 	if (!new_env)
// 		return;
// 	i = 0;
// 	while (i < size)
// 	{
// 		new_env[i] = (*env)[i];
// 		i++;
// 	}
// 	new_env[size] = ft_strdup(new_var);
//  	new_env[size + 1] = NULL;
// 	free(*env);
// 	*env = new_env;
// }
