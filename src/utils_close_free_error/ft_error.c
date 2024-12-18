/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:22:52 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 18:24:26 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error_exit(char *str, int exit_c)
{
	perror(str);
	exit(exit_c);
}

int	ft_error_int(char *str, t_parser *node)
{
	ft_putendl_fd(str, 2);
	node->exit_code = 1;
	return (-1);
}

void	free_exit_tab_str(char **env, char **cmd, char *other_cmd, int code)
{
	if (cmd[0])
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": not executable", 2);
		free_array(cmd);
	}
	else
		ft_putendl_fd("execve failed", 2);
	if (env)
		free_array(env);
	if (other_cmd)
		free(other_cmd);
	exit(code);
}

void	msg_not_executable(char *str)
{
	ft_putstr_fd(str, 2);
	ft_putendl_fd(": not executable", 2);
}
