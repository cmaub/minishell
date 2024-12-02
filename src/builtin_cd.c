/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/02 17:18:36 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
si cd est appele sans arguments -> fail (cf sujet)
si plus d'un argument -> fail

chdir() permet de changer de repertoire. 
Retourne 0 en cas de succes et -1 si elle echoue

avant de changer de repertoire sauvegarder PWD dans OLDPWD
si le changement reussi, mettre a jour la nouvelle position

char *getcwd( char *buffer, size_t size );
permet de recuperer le chemin absolu du repertoire courant
- buffer : le bloc de mémoire dans lequel le chemin de travail courant vous sera retourné.
- size : la taille du bloc de mémoire passé en premier paramètre.
*/


int	ft_setenv(char *dest, char *src, char **mini_env)
{	
	dest = ft_strjoin(dest, "=");
	if (!dest)
		return (-1);
	while (*mini_env && ft_strnstr(*mini_env, dest, ft_strlen(dest)) == NULL)
		mini_env++;
	if (*mini_env == NULL)
	{
		if (ft_strncmp(dest, "OLDPWD=", 6) == 0)
		{
			*mini_env = try_malloc(sizeof(char *));
			*mini_env = ft_strjoin(dest, ft_strdup(src));
			if (!*mini_env)
				return (free(dest), -1);
		}
		else
			return (-1);
	}
	else if (*mini_env)
	{
		*mini_env = ft_strjoin(dest, ft_strdup(src));
		if (!*mini_env)
			return (free(dest), -1);
	}
	return (0);
}

int	ft_cd(char **cmd, t_pipex *p, PARSER *node)
{
	char	*old_pwd;
	char	*new_pwd;

	if (!cmd[1])
	{
		node->exit_code = 1;
		return (ft_putstr_fd("cd: no directory specified\n", 2), -1);
	}
	if (cmd[2])
		return (ft_error_int("cd", node));
	else
	{
		if (chdir(cmd[1]) == -1)
		{
			dprintf(2, "chdir echoue");
			return (ft_error_int("cd", node));
		}
	}
	if (env_var_exists(p->mini_env, "PWD") == -1)
	{
		node->exit_code = 1;
		ft_putstr_fd("path not found\n", 2);
		return (-1);
	}
	old_pwd = ft_strdup(return_var_from_env("PWD", p->mini_env));
	if (!old_pwd)
		return (ft_error_int("cd", node));
	if (old_pwd)
		ft_setenv("OLDPWD", old_pwd, p->mini_env);
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
		return (ft_setenv("PWD", new_pwd, p->mini_env), TRUE);
	else
		return (ft_error_int("getcwd", node));
}
