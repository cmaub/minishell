/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:32:16 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/18 15:32:31 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **mini_env)
{
	(void)mini_env;
	char	*new_var;
	
	new_var = NULL;
	new_var = getcwd(NULL, 0);
	if (!new_var)
	{
		perror("pwd");
		return (FALSE);
	}
	// while (*mini_env && ft_strnstr(*mini_env, "PWD=", 4) == NULL)
	// 	mini_env++;
	// if (*mini_env == NULL)
	// {
	// 	return (-1);
	// }
	// new_var = ft_strdup(*mini_env + 4);
	ft_putstr_fd(new_var, 1);
	write(1, "\n", 1);
	return (TRUE);
}
