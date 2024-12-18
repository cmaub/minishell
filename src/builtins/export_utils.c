/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:08:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 14:11:34 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	is_ignored_var(char *var)
{
	if (!var)
		return (0);
	return (var[0] == '_' && (var[1] == '\0' || var[1] == '='));
}

void	print_sorted_env(t_env **env_n)
{
	int		i;
	t_env	*cur;

	i = 0;
	if (!*env_n || !env_n)
		return ;
	cur = *env_n;
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
	free_env(sorted_env);
}
