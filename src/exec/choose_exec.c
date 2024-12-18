/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:38:37 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 16:03:20 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	try_find_cmd_file(char **tmp_cmd, char **str_env)
{
	char	*dir_cmd;
	char	*curr_dir;

	curr_dir = "./";
	dir_cmd = ft_strjoin(curr_dir, tmp_cmd[0]);
	if (!dir_cmd)
		free_exit_tab_str(tmp_cmd, str_env, NULL, 1);
	if (access(dir_cmd, F_OK) == 0)
	{
		if (access(dir_cmd, R_OK) == -1)
			free_exit_tab_str(str_env, tmp_cmd, dir_cmd, 126);
		if (find_path(str_env))
		{
			if (execve(dir_cmd, tmp_cmd, str_env) == -1)
				free_exit_tab_str(str_env, tmp_cmd, dir_cmd, 127);
		}
		else
			if (execve(dir_cmd, tmp_cmd, NULL) == -1)
				free_exit_tab_str(str_env, tmp_cmd, dir_cmd, 126);
	}
	else
		free_exit_tab_str(str_env, tmp_cmd, dir_cmd, 127);
}

void	exec_no_env_or_path(char **tmp_cmd, char **str_env)
{
	if (access(tmp_cmd[0], F_OK) == 0)
	{
		if (access(tmp_cmd[0], R_OK) == -1)
			(free_exit_tab_str(str_env, tmp_cmd, NULL, 126));
		if (execve(tmp_cmd[0], tmp_cmd, NULL) == -1)
			(free_exit_tab_str(str_env, tmp_cmd, NULL, 126));
		exit(126);
	}
	else if (!ft_strchr(tmp_cmd[0], '/'))
		try_find_cmd_file(tmp_cmd, str_env);
	else
		(free_exit_tab_str(str_env, tmp_cmd, NULL, 127));
}

void	exec_without_pb(char **tmp_cmd, char **str_env)
{
	char	*path;

	path = get_path_and_check(&tmp_cmd[0], str_env);
	if (execve(path, tmp_cmd, str_env) == -1)
	{
		ft_putstr_fd(tmp_cmd[0], 2);
		ft_putendl_fd(": not executable", 2);
		free_array(tmp_cmd);
		free(path);
		free_array(str_env);
	}
	exit(126);
}

int	execute(t_parser **cur, t_pipex *p, t_mega *m)
{
	char	**tmp_cmd;
	char	**tmp_minienv;
	char	**str_env;

	tmp_cmd = NULL;
	tmp_minienv = NULL;
	str_env = NULL;
	if (is_builtin(*cur) == 1)
		(exc_built(*cur, p, NULL, m), rst_nde(&m->begin), free_env(p->env_n),
			free_pipex(&p), free(m), exit(EXIT_SUCCESS));
	else
	{
		str_env = copy_list_in_str(p->env_n);
		if (!str_env)
			return (FALSE);
		tmp_cmd = copy_tab((*cur)->command);
		if (!tmp_cmd)
			return (free_array(str_env), FALSE);
		(free_env(p->env_n), free_pipex(&p), rst_nde(&m->begin), free(m));
		if (ft_strchr(tmp_cmd[0], '/') || !find_path(str_env))
			exec_no_env_or_path(tmp_cmd, str_env);
		else if (!ft_strchr(tmp_cmd[0], '/') && find_path(str_env))
			exec_without_pb(tmp_cmd, str_env);
	}
	return (FALSE);
}
