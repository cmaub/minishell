/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/09 11:44:45 by cmaubert         ###   ########.fr       */
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


int	ft_setenv(char *dest, char *src, t_env **env_nodes)
{	
	t_env 	*temp;

	temp = *env_nodes;
	dest = ft_strjoin(dest, "=");
	if (!dest)
		return (-1);
	while (temp && ft_strnstr(temp->var, dest, ft_strlen(dest)) == NULL)
		temp = temp->next;
	if (temp->var == NULL)
	{
		if (ft_strncmp(dest, "OLDPWD=", 6) == 0)
		{
			temp->var = try_malloc(sizeof(char *));
			temp->var = ft_strjoin(dest, ft_strdup(src));
			if (!temp->var)
				return (free(dest), -1);
		}
		else
			return (-1);
	}
	else if (temp->var)
	{
		temp->var = ft_strjoin(dest, ft_strdup(src));
		if (!temp->var)
			return (free(dest), -1);
	}
	return (0);
}

// int	ft_setenv(char *dest, char *src, char **mini_env)
// {	
// 	dest = ft_strjoin(dest, "=");
// 	if (!dest)
// 		return (-1);
// 	while (*mini_env && ft_strnstr(*mini_env, dest, ft_strlen(dest)) == NULL)
// 		mini_env++;
// 	if (*mini_env == NULL)
// 	{
// 		if (ft_strncmp(dest, "OLDPWD=", 6) == 0)
// 		{
// 			*mini_env = try_malloc(sizeof(char *));
// 			*mini_env = ft_strjoin(dest, ft_strdup(src));
// 			if (!*mini_env)
// 				return (free(dest), -1);
// 		}
// 		else
// 			return (-1);
// 	}
// 	else if (*mini_env)
// 	{
// 		*mini_env = ft_strjoin(dest, ft_strdup(src));
// 		if (!*mini_env)
// 			return (free(dest), -1);
// 	}
// 	return (0);
// }

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
	{
		node->exit_code = 1;
		return (ft_putstr_fd("too many arguments", 2), -1);
	}
	else
	{
		if (chdir(cmd[1]) == -1)
		{
			//dprintf(2, "chdir echoue");
			return (ft_error_int("cd", node));
		}
	}
	if (env_var_exists(p->env_nodes, "PWD") == -1)
	{
		node->exit_code = 1;
		ft_putstr_fd("path not found\n", 2);
		return (-1);
	}
	old_pwd = ft_strdup(return_var_from_env("PWD", p->env_nodes));
	if (!old_pwd)
		return (ft_error_int("cd", node));
	if (old_pwd)
		ft_setenv("OLDPWD", old_pwd, p->env_nodes);
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
		return (ft_setenv("PWD", new_pwd, p->env_nodes), TRUE);
	else
		return (ft_error_int("getcwd", node));
}
