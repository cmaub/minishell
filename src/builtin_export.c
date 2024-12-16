/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:04:35 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/16 14:37:00 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export rend une variable d'environnement accessible aux processus enfants.
// il permet de rajouter une ou plusieurs variables d'environnement
// si elles existent deja, il est possible de changer leur valeur
// si export sans arg on affiche les variables d'env dans l'ordre ascii
// verifier la syntaxe des noms de variables + des valeurs possibles
// gerer les guillemet et les "export" ecrits avant

// int	env_var_exists(t_env **env_nodes, char *var)
// {
// 	int		i;
// 	int		size;
// 	int		sizeofvar;
// 	t_env	*temp;

// 	i = 0;
// 	size = lstsize_t_env(env_nodes);
// 	sizeofvar = ft_strlen(var);
// 	temp = *env_nodes;
// 	if (!var || var[0] == '\0')
// 		return (-1);
// 	if (!env_nodes || !(*env_nodes))
// 		return (-1);
// 	while (temp && i < size)
// 	{
// 		if (ft_strncmp(temp->var, var, sizeofvar) == 0
// 			&& (temp->var[sizeofvar] == '\0'
// 				|| temp->var[sizeofvar] == '='))
// 			return (i);
// 		i++;
// 		temp = temp->next;
// 	}
// 	return (-1);
// }

void	sort_tab_ascii(t_env **env)
{
	char	*temp;
	t_env	*current;
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
	int		j;
	char	*tmp;

	j = 0;
	ft_putstr_fd("export ", 1);
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

int	is_ignored_var(char *var)
{
	if (!var)
		return (0);
	return (var[0] == '_' && (var[1] == '\0' || var[1] == '='));
}

void	print_sorted_env(t_env **env_nodes)
{
	int		i;
	t_env	*cur;

	i = 0;
	if (!*env_nodes || !env_nodes)
		return ;
	cur = *env_nodes;
	while (cur != NULL)
	{
		if (is_ignored_var(cur->var))
		{
			if (cur->next)
				cur = cur->next;
			else
				break ;
		}
		else if (cur && cur->var)
		{
			write_var(cur->var);
			if (cur->next)
				cur = cur->next;
			else
				break ;
		}			
	}
}

t_env	**copy_t_env(t_env **env)
{
	t_env	**sorted_env;
	t_env	*new_var;
	t_env	*current;
	int		count;

	current = *env;
	sorted_env = try_malloc(sizeof(t_env *));
	if (!sorted_env)
		return (env);
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
	free_t_env(sorted_env);
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

void	print_error_msg(char *str, PARSER *current, char *error_msg)
{
	ft_putstr_fd("export: '", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd(error_msg, 2);
	current->exit_code = 1;
}

// int	create_new_var(t_env **env_nodes, char *cmd)
// {
// 	t_env	*new;

// 	new = try_malloc(sizeof(t_env));
// 	if (!new)
// 		return (FALSE);
// 	new->var = ft_strdup(cmd);
// 	if (!new->var)
// 		return (free(new), FALSE);
// 	new->next = NULL;
// 	ft_lstadd_env_back(env_nodes, new);
// 	return (TRUE);
// }

int	check_value_and_add(t_env **env_nodes, char *cmd, int index)
{
	t_env	*temp;
	int		j;

	temp = *env_nodes;
	j = -1;
	if (index >= 0)
	{
		while (temp && j++ < index)
			temp = temp->next;
		if (!temp)
			return (FALSE);
		free(temp->var);
		temp->var = ft_strdup(cmd);
		if (!temp->var)
			return (FALSE);
	}
	else
		return (create_new_var(env_nodes, cmd));
	return (TRUE);
}

void	handle_value(char *cmd, PARSER *current, t_env **env_nodes)
{
	int		index;
	char	*equal;
	char	*name;

	equal = ft_strchr(cmd, '=');
	*equal = '\0';
	name = cmd;
	if (!check_name(name))
		return (print_error_msg(name, current, "': not a valid identifier"));
	index = env_var_exists(env_nodes, name);
	*equal = '=';
	dprintf(2, "cmd = %s\n", cmd);
	if (!check_value_and_add(env_nodes, cmd, index))
		return ;
}

t_env	**handle_var_wo_value(char *cmd, PARSER *current, t_env **env_nodes)
{
	int		index;
	int		j;
	t_env	*new_var;

	j = 0;
	new_var = NULL;
	if (!check_name(cmd))
	{
		print_error_msg(cmd, current, "': not a valid identifier");
		return (env_nodes);
	}
	index = env_var_exists(env_nodes, cmd);
	if (index < 0)
		create_new_var(env_nodes, cmd);
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
			print_error_msg(current->command[i], current, ": invalid option");
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
			handle_var_wo_value(current->command[i], current, env_nodes);
		i++;
	}
	return (env_nodes);
}
