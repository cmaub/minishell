/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:10:16 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 18:16:14 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_tmp_filename(char *path, char *cmd)
{
	char	*tmp_filename;
	char	*tmp_path;

	tmp_path = ft_strjoin(path, "/");
	if (!tmp_path)
		return (NULL);
	tmp_filename = ft_strjoin(tmp_path, cmd);
	if (!tmp_filename)
		return (free(tmp_path), NULL);
	free(tmp_path);
	return (tmp_filename);
}

char	**find_path_line(char **env)
{
	char	**new_path;

	while (*env && ft_strnstr(*env, "PATH", 4) == 0)
		env++;
	if (!*env)
		return (NULL);
	new_path = ft_split(*env + 5, ':');
	if (!new_path)
	{
		perror("path");
		exit(EXIT_FAILURE);
	}
	return (new_path);
}

char	*get_path(char *cmd, char **env)
{
	char			**path;
	char			*tmp_filename;
	char			*filename;
	unsigned long	i;

	filename = NULL;
	i = 0;
	path = find_path_line(env);
	if (!path)
		return (NULL);
	while (path[i])
	{
		tmp_filename = create_tmp_filename(path[i++], cmd);
		if (!tmp_filename)
			return (free_array(path), NULL);
		if (access(tmp_filename, F_OK) == 0)
		{
			if (access(tmp_filename, R_OK) == -1)
				return (free_array(path), free(tmp_filename), NULL);
			filename = tmp_filename;
			break ;
		}
		free(tmp_filename);
	}
	return (free_array(path), filename);
}

char	*get_path_and_check(char **split_cmd, char **env)
{
	char	*new_path;

	new_path = get_path(split_cmd[0], env);
	if (!new_path)
		try_find_cmd_file(split_cmd, env);
	return (new_path);
}
