/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:32:16 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/09 14:35:01 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(PARSER *current, t_env **env)
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
