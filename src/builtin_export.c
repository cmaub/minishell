/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:04:35 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/06 16:41:04 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export rend une variable d'environnement accessible aux processus enfants.
// il permet de rajouter une ou plusieurs variables d'environnement, en les initialisant ou non
// il faut donc verifier si elles existent deja avant de les creer
// si elles existent deja, il est possible de changer la valeur de la variable d'env deja existante
// si export sans arg on affiche les variables d'environnement dans l'ordre ascii
// verifier la syntaxe des noms de variables + des valeurs possibles
// gerer les guillemet et les "export" ecrits avant

int		env_var_exists(char **env, char *var)
{
	int	i;
	int	size;
	int	sizeofvar;

	i = 0;
	size = count_env_var(env);
	sizeofvar = ft_strlen(var);
	//dprintf(2, "size dans env_var_exist = %d\n", size);
	if (!var || var[0] == '\0')
		return (-1);
	if (!env || !(*env))
	{
		//dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
		return (-1);
	}
	while (env[i] && i < size)
	{
		if (ft_strncmp(env[i], var, sizeofvar + 1) == 0)
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
		//dprintf(2, "env[%d] = %s\n", i, env[i]);
		i++;
	}
}

void	write_var(char *sorted_env)
{
	int	j;
	char	*tmp;

	j = 0;
	// dprintf(2, "str dans sorted_env = %s\n", sorted_env);
	ft_putstr_fd("export ", 2);
	while (*sorted_env && sorted_env[j] && sorted_env[j] != '=')
	{
		write(1, &sorted_env[j], 1);
		j++;
	}
	if (sorted_env[j] == '=')
	{
		write(1, "=\"", 2);
		tmp = ft_strnchr(sorted_env, '=', ft_strlen(sorted_env));
		if (tmp != NULL)
			ft_putstr_fd(tmp + 1, 1);
		write(1, "\"\n", 2);
	}
	else
		write(1, "\n", 1);
}

void	print_sorted_env(char **sorted_env)
{
	int	i;

	i = 0;
	while (sorted_env[i] != NULL)
	{
		if (sorted_env[i] && sorted_env[i][0] == '_' && (sorted_env[i][1] == '\0' || sorted_env[i][1] == '='))
		{
			if (sorted_env[i + 1])
				i++;
			else
				break ;
		}
		else if (*sorted_env && sorted_env[i])
		{
			write_var(sorted_env[i]);
			i++;
		}			
	}
}

char	**copy_plus_one_malloc(char **env)
{
	int		i;
	char	**sorted_env;
	int		count;

	count = 0;
	i = 0;
	sorted_env = NULL;
	while (env[count])
		count++;
	sorted_env = try_malloc((count + 1) * sizeof(char *));
	while (env[i] != NULL)
	{
		sorted_env[i] = ft_strdup(env[i]);
		i++;
	}
	sorted_env[i] = NULL;
	return (sorted_env);
}
void	copy_and_sort_env(char **env)
{
	int	count;
	char	**sorted_env;

	sorted_env = copy_plus_one_malloc(env);
	count = count_env_var(sorted_env);
	sort_tab_ascii(sorted_env, count);
	print_sorted_env(sorted_env);
	//dprintf(2, "**** ici = (%s, %d)\n", __FILE__, __LINE__);
	free_array(sorted_env);
	//dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
}

int	check_name(char *name)
{
	int	i;

	i = 0;
	if (!name || !name[0] || ft_isdigit(name[0]))
		return (FALSE);
	// //dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	// //dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
	return (TRUE);
}

void	print_error_msg(char *str, PARSER *current)
{
	ft_putstr_fd("minishell: export: '", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	current->exit_code = 1;
}

void	handle_value(char *cmd, PARSER *current, char **env)
{
	int		index;
	char	*equal;
	char	*name;
	int		j;

	equal = ft_strchr(cmd, '=');
	*equal = '\0';
	name = cmd;
	j = 0;
	if (!check_name(name))
		return (print_error_msg(name, current));
	index = env_var_exists(env, name);
	*equal = '=';
	if (index >= 0)
	{
		//dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
		free(env[index]);
		//dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
		env[index] = ft_strdup(cmd);
	}
	else
	{
		while (env[j])
			j++;
		env[j] = ft_strdup(cmd);
		env[j + 1] = NULL;
	}
}

// char	**handle_variable_without_value(char *cmd, PARSER *current, char **env)
// {
// 	int index;
// 	int j;
// 	char	**new_env;

// 	j = 0;
// 	if (!check_name(cmd))
// 	{
// 		print_error_msg(cmd, current);
// 		return (env);
// 	}
// 	new_env = copy_plus_one_malloc(env);
// 	index = env_var_exists(new_env, cmd);
// 	dprintf(2, "index = %d\n", index);
// 	if (index < 0)
// 	{
// 		//dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
// 		while (new_env[j])
// 			j++;
// 		new_env[j] = ft_strdup(cmd);
// 		dprintf(2, "env[%d] = %s\n", j, new_env[j]);
// 		if (!new_env[j])
// 			return (env);
// 		new_env[j + 1] = NULL;
// 		//dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
// 	}
// 	dprintf(2, "dans handle_variable_without_value ** cmd = %s\n", cmd);
// 	// free_array(env);
// 	//free tab env
// 	return (new_env);
// }

void	handle_variable_without_value(char *cmd, PARSER *current, char **env)
{
	int index;
	int j;

	j = 0;
	if (!check_name(cmd))
	{
		print_error_msg(cmd, current);
		return ;
	}
	index = env_var_exists(env, cmd);
	
	// dprintf(2, "index = %d\n", index);
	if (index < 0)
	{
		//dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
		while (env[j])
			j++;
		env[j] = ft_strdup(cmd);
		dprintf(2, "env[%d] = %s\n", j, env[j]);
		if (!env[j])
			return ;
		env[j + 1] = NULL;
		//dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
	}
	// dprintf(2, "dans handle_variable_without_value ** cmd = %s\n", cmd);
}

int	ft_export(PARSER *current, char **env)
{
	int	i;

	i = 1;
	if (!current->command[1]) // si pas d'arg afficher par ordre ascii
	{
		dprintf(2, "size dans env_var_exist = %d\n", count_env_var(env));
		print_sorted_env(env);
		return (TRUE);
		// return (copy_and_sort_env(env), TRUE);
	}
	while (current->command[i])
	{
		//dprintf(2, "haut ** current->command[%d] = %s\n", i, current->command[i]);
		if (current->command[i] && current->command[i][0] == '-')
		{
			ft_putstr_fd("export:", 2);
			ft_putstr_fd(current->command[i], 2);
			ft_putendl_fd(": invalid option", 2);
			current->exit_code = 1;
			if (i == 1)
			{
				current->exit_code = 2;
				return (FALSE);
			}				
		}
		//dprintf(2, "bas ** current->command[%d] = %s\n", i, current->command[i]);
		if (ft_strchr(current->command[i], '=') != NULL)
			handle_value(current->command[i], current, env);
		else
			handle_variable_without_value(current->command[i], current, env);
		// print_sorted_env(new_tab);
		i++;
	}
	return (TRUE);
}

// int	ft_export(PARSER *current, char **env)
// {
// 	int	i;
// 	char	**new_tab;

// 	i = 1;
// 	new_tab = NULL;
// 	if (!current->command[1]) // si pas d'arg afficher par ordre ascii
// 	{
// 		dprintf(2, "size dans env_var_exist = %d\n", count_env_var(env));
// 		// print_sorted_env(env);
// 		// return (TRUE);
// 		return (copy_and_sort_env(env), TRUE);
// 	}
// 	while (current->command[i])
// 	{
// 		new_tab = copy_plus_one_malloc(env);
// 		//dprintf(2, "haut ** current->command[%d] = %s\n", i, current->command[i]);
// 		if (current->command[i] && current->command[i][0] == '-')
// 		{
// 			ft_putstr_fd("export:", 2);
// 			ft_putstr_fd(current->command[i], 2);
// 			ft_putendl_fd(": invalid option", 2);
// 			current->exit_code = 1;
// 			if (i == 1)
// 			{
// 				current->exit_code = 2;
// 				return (FALSE);
// 			}				
// 		}
// 		//dprintf(2, "bas ** current->command[%d] = %s\n", i, current->command[i]);
// 		if (ft_strchr(current->command[i], '=') != NULL)
// 			handle_value(current->command[i], current, new_tab);
// 		else
// 			handle_variable_without_value(current->command[i], current, new_tab);
// 		// print_sorted_env(new_tab);
// 		env = copy_tab(new_tab);
// 		i++;
// 	}
// 	return (TRUE);
// }