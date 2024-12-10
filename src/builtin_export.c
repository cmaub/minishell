/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:04:35 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/10 12:19:54 by anvander         ###   ########.fr       */
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

int		env_var_exists(t_env **nodes_env, char *var)
{
	int	i;
	int	size;
	int	sizeofvar;
	t_env	*temp;

	i = 0;
	size = lstsize_t_env(nodes_env);
	sizeofvar = ft_strlen(var);
	temp = *nodes_env;
	if (!var || var[0] == '\0')
		return (-1);
	if (!nodes_env || !(*nodes_env))
		return (-1);
	while (temp && i < size)
	{
		if (ft_strncmp(temp->var, var, sizeofvar) == 0 && (temp->var[sizeofvar] == '\0' || temp->var[sizeofvar] == '='))
			return (i);
		
		i++;
		temp = temp->next;
	}
	return (-1);
}

void	sort_tab_ascii(t_env **env)
{
	char *temp;
	t_env *current;
	t_env	*head;

	head = *env;
	current = head;
	temp = NULL;
	while (current != NULL && current->next != NULL)
	{
		if (ft_strcmp(current->var, current->next->var) > 0)
		{
			temp = current->var;
			current->var = current->next->var;
			current->next->var = temp;
			current = head;
		}
		else
			current = current->next;
	}
}

void	write_var(char *sorted_env)
{
	int	j;
	char	*tmp;

	j = 0;
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

void	print_sorted_env(t_env **env_nodes)
{
	int	i;
	t_env *current;

	i = 0;
	if (!*env_nodes || !env_nodes)
		return ;
	current = *env_nodes;
	while (current != NULL)
	{
		if (current->var && current->var[0] == '_' && (current->var[1] == '\0' || current->var[1] == '='))
		{
			if (current->next)
				current = current->next;
			else
				break ;
		}
		else if (current && current->var)
		{
			write_var(current->var);
			if (current->next)
				current = current->next;
			else
				break ;
		}			
	}
}
// void	print_sorted_env(t_env **env_nodes)
// {
// 	int	i;

// 	i = 0;
// 	while (sorted_env[i] != NULL)
// 	{
// 		if (sorted_env[i] && sorted_env[i][0] == '_' && (sorted_env[i][1] == '\0' || sorted_env[i][1] == '='))
// 		{
// 			if (sorted_env[i + 1])
// 				i++;
// 			else
// 				break ;
// 		}
// 		else if (*sorted_env && sorted_env[i])
// 		{
// 			write_var(sorted_env[i]);
// 			i++;
// 		}			
// 	}
// }

t_env	**copy_t_env(t_env **env)
{
	t_env	**sorted_env = NULL;
	t_env	*new_var;
	t_env	*current;
	int	count;
	
	current = *env;
	sorted_env = try_malloc(sizeof(t_env *));
	count = lstsize_t_env(&current);
	while (current != NULL)
	{
		new_var = try_malloc(sizeof(t_env));
		if (!new_var)
			return (env);
		new_var->var = ft_strdup(current->var);
		if (!new_var)
			return (env);
		new_var->next = NULL;
		add_new_var(sorted_env, new_var);
		current = current->next;
	}
	return (sorted_env);
}

void	copy_and_sort_env(t_env **env)
{
	t_env	**sorted_env;

	sorted_env = copy_t_env(env);
	
	sort_tab_ascii(sorted_env);
	print_sorted_env(sorted_env);
	free_t_env(sorted_env); // free la liste
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

void	handle_value(char *cmd, PARSER *current, t_env **nodes_env)
{
	int		index;
	char	*equal;
	char	*name;
	int		j;
	t_env	*temp;
	t_env	*new;

	equal = ft_strchr(cmd, '=');
	*equal = '\0';
	name = cmd;
	j = 0;
	new = NULL;
	if (!check_name(name))
		return (print_error_msg(name, current));
	index = env_var_exists(nodes_env, name);
	*equal = '=';
	temp = *nodes_env;
	if (index >= 0)
	{
		while (temp && j < index)
		{
			temp = temp->next;
			j++;
		}
		free(temp->var);
		temp->var = ft_strdup(cmd);
		if (!temp->var)
			return ;
	}
	else
	{
		new = try_malloc(sizeof(t_env));
		new->var = ft_strdup(cmd);
		new->next = NULL; 
		if (!new->var)
			return ;
		ft_lstadd_env_back(nodes_env, new);
	}
}

t_env	**handle_variable_without_value(char *cmd, PARSER *current, t_env **env_nodes)
{
	int index;
	int j;
	t_env	*new_var;

	j = 0;
	new_var = NULL;
	if (!check_name(cmd))
	{
		print_error_msg(cmd, current);
		return (env_nodes);
	}
	index = env_var_exists(env_nodes, cmd);
	if (index < 0)
	{
		new_var = try_malloc(sizeof(t_env));
		new_var->var = ft_strdup(cmd);
		new_var->next = NULL; 
		add_new_var(env_nodes, new_var);
	}
	return (env_nodes);
}

t_env	**ft_export(PARSER *current, t_env **env_nodes)
{
	int	i;

	i = 1;
	if (!current->command[1])
		return (copy_and_sort_env(env_nodes), env_nodes);
	while (current->command[i])
	{
		if (current->command[i] && current->command[i][0] == '-')
		{
			ft_putstr_fd("export:", 2);
			ft_putstr_fd(current->command[i], 2);
			ft_putendl_fd(": invalid option", 2);
			current->exit_code = 1;
			if (i == 1)
			{
				current->exit_code = 2;
				return (env_nodes);
			}				
		}
		if (ft_strchr(current->command[i], '=') != NULL)
			handle_value(current->command[i], current, env_nodes);
		else
			handle_variable_without_value(current->command[i], current, env_nodes);
		i++;
	}
	return (env_nodes);
}
