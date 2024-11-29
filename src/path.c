/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:10:16 by anvander          #+#    #+#             */
/*   Updated: 2024/11/29 16:51:25 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_tmp_filename(char *path, char *cmd)
{
	char	*tmp_filename;
	char	*tmp_path;

	tmp_path = ft_strjoin(path, "/");
	if (!tmp_path)
		return (free(path), NULL);
	tmp_filename = ft_strjoin(tmp_path, cmd);
	free(tmp_path);
	if (!tmp_filename)
		return (free(path), NULL);
	return (tmp_filename);
}

char	**find_path_line(char **env)
{
	char	**new_path;

	while (*env && ft_strnstr(*env, "PATH", 4) == 0)
		env++;
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
	while (path[i])
	{
		tmp_filename = create_tmp_filename(path[i], cmd);
		if (!tmp_filename)
			return (ft_free_tab(path), NULL);
		if (access(tmp_filename, F_OK) == 0)
		{
			if (access(tmp_filename, R_OK) == -1)
				exit (126);
			filename = tmp_filename;
			break ;
		}
		free(tmp_filename);
		i++;
	}
	return (ft_free_tab(path), filename);
}

char	*get_path_and_check(char **split_cmd, char **env)
{
	char	*new_path;
	
	new_path = get_path(split_cmd[0], env);
	if (!new_path)
	{
		ft_putstr_fd(split_cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		ft_free_tab(split_cmd); //peut-etre pas besoin de free
		exit (127); //
	}
	return (new_path);
}


