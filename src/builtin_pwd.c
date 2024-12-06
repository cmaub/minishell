/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:32:16 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/06 12:22:45 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(PARSER *current)
{
	char	*new_var;
	
	new_var = NULL;
	if (current->command[1] && current->command[1][0] == '-')
	{
		ft_putstr_fd("pwd:", 2);
		ft_putstr_fd(current->command[1], 2);
		ft_putendl_fd(": invalid option", 2);
		current->exit_code = 2;
		return (FALSE);
	}
	new_var = getcwd(NULL, 0);
	if (!new_var)
	{
		perror("pwd");
		current->exit_code = 1;
		return (FALSE);
	}
	ft_putstr_fd(new_var, 1);
	write(1, "\n", 1);
	return (TRUE);
}
