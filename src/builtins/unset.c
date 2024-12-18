/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:33:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 14:20:21 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Deletes one or more environment variables
// If a variable does not exist, unset must not cause the program to terminate

#include "minishell.h"

int	count_env_var(char **list)
{
	int	len;

	len = 0;
	while (list[len] != NULL)
	{
		len++;
	}
	return (len);
}

int	check_args_unset(t_parser *current, t_env **env_n)
{
	if (!env_n || !*env_n)
	{
		env_n = NULL;
		return (FALSE);
	}
	if (!current->command[1])
		return (FALSE);
	if (current->command[1] && current->command[1][0] == '-')
	{
		(ft_putstr_fd("unset: ", 2), ft_putstr_fd(current->command[1], 2),
			ft_putendl_fd(": invalid option", 2));
		current->exit_code = 2;
		return (FALSE);
	}
	return (TRUE);
}

t_env	**browse_env_and_unset_var(t_env **env_n, int index)
{
	int		j;
	t_env	*temp;
	t_env	*saved;

	j = -1;
	temp = *env_n;
	saved = NULL;
	if (index == 0)
	{
		*env_n = temp->next;
		(free(temp->var), free(temp));
	}
	else
	{
		while (temp && j++ < index -1)
		{
			saved = temp;
			temp = temp->next;
		}
		if (!temp)
			return (env_n);
		saved->next = temp->next;
		(free(temp->var), free(temp));
	}
	return (env_n);
}

t_env	**ft_unset(t_parser *current, t_env **env_n)
{
	int	i;
	int	index;
	int	size_env;

	i = 1;
	if (!check_args_unset(current, env_n))
		return (env_n);
	while (current->command[i] != NULL)
	{
		index = env_var_exists(env_n, current->command[i]);
		if (index >= 0)
		{
			size_env = lstsize_t_env(env_n);
			if (size_env == 1)
			{
				ft_putendl_fd("empty envp not allowed\n", 2);
				break ;
			}
			env_n = browse_env_and_unset_var(env_n, index);
		}
		i++;
	}
	return (env_n);
}
