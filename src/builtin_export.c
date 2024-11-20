/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:04:35 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/20 12:22:50 by anvander         ###   ########.fr       */
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

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, strlen(var)) == 0)
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

void	write_var(char *sorted_env)
{
	int	j;
	char	*tmp;

	j = 0;
	ft_putstr_fd("export ", 2);
	while (sorted_env[j] && sorted_env[j] != '=')
		write(1, &sorted_env[j++], 1);
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
		write_var(sorted_env[i]);
		i++;
	}
}

void	copy_and_sort_env(char **env)
{
	int		count;
	int		i;
	char	**sorted_env;

	count = 0;
	i = 0;
	sorted_env = NULL;
	while (env[count])
		count++;
	sorted_env = try_malloc((count + 1) * sizeof(char *));
	while (i < count)
	{
		sorted_env[i] = ft_strdup(env[i]);
		i++;
	}
	sorted_env[count] = NULL;
	sort_tab_ascii(sorted_env, count);
	print_sorted_env(sorted_env);
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
		free(env[index]);
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
	if (index < 0)
	{
		while (env[j])
			j++;
		env[j] = ft_strdup(cmd);
		env[j + 1] = NULL;
	}
}

int	ft_export(PARSER *current, char **env)
{
	int	i;

	i = 1;
	if (!current->command[1]) // si pas d'arg afficher par ordre ascii
		return (copy_and_sort_env(env), TRUE);
	while (current->command[i])
	{
		if (current->command[i][0] == '-')
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
		if (ft_strchr(current->command[i], '=') != NULL)
			handle_value(current->command[i++], current, env);
		else
			handle_variable_without_value(current->command[i++], current, env);
	}
	return (TRUE);
}
