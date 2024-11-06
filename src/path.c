/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:10:16 by anvander          #+#    #+#             */
/*   Updated: 2024/11/06 10:48:55 by anvander         ###   ########.fr       */
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
		exit (EXIT_FAILURE);
	}
	return (new_path);
}

// Test every path until the command's one is found
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
		if (access(tmp_filename, F_OK | R_OK) == 0)
		{
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
	dprintf(2, "path = %s\n", new_path);
	if (!new_path)
	{
		dprintf(2, "new_path n'existe pas\n");
		ft_free_tab(split_cmd);
		exit(EXIT_FAILURE);
	}
	dprintf(2, "new_path valide\n");
	return (new_path);
}


