/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:32:16 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/13 19:05:51 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_new_var(t_env **node, char *str)
{
	t_env	*new;

	new = try_malloc(sizeof(t_env));
	if (!new)
		return (FALSE);
	new->var = ft_strdup(str);
	if (!new->var)
		return (FALSE);
	new->next = NULL;
	ft_lstadd_env_back(node, new);
	return (TRUE);
}

int	env_var_exists(t_env **env_n, char *var)
{
	int		i;
	int		size;
	int		sizeofvar;
	t_env	*temp;

	i = 0;
	size = lstsize_t_env(env_n);
	sizeofvar = ft_strlen(var);
	temp = *env_n;
	if (!var || var[0] == '\0')
		return (-1);
	if (!env_n || !(*env_n))
		return (-1);
	while (temp && i < size)
	{
		if (ft_strncmp(temp->var, var, sizeofvar) == 0
			&& (temp->var[sizeofvar] == '\0'
				|| temp->var[sizeofvar] == '='))
			return (i);
		i++;
		temp = temp->next;
	}
	return (-1);
}

int	ft_env(t_parser *current, t_env **env)
{
	t_env	*curr;

	curr = *env;
	if (current->command[1])
	{
		ft_putstr_fd("Too many arguments\n", 2);
		current->exit_code = 2;
		return (FALSE);
	}
	while (curr)
	{
		if (ft_strchr(curr->var, '='))
		{
			ft_putstr_fd(curr->var, 1);
			write(1, "\n", 1);
		}
		curr = curr->next;
	}
	return (TRUE);
}
