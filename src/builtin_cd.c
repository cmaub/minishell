/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/10 15:30:21 by anvander         ###   ########.fr       */
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
	temp->var = ft_strjoin(dest, ft_strdup(src));
	if (!temp->var)
		return (free(dest), -1);
	return (0);
}

int	ft_cd(char **cmd, t_pipex *p, PARSER *node)
{
	char	*old_pwd;
	char	*new_pwd;

	if (!cmd[1])
		return(ft_error_int("cd: no directory specified", node));
	if (cmd[2])
		return(ft_error_int("cd: too many arguments", node));
	else
	{
		if (chdir(cmd[1]) == -1)
			return (ft_error_int("cd: error", node));
	}
	if (env_var_exists(p->env_nodes, "PWD") == -1)
		return(ft_error_int("cd: path not found", node));
	old_pwd = ft_strdup(return_var_from_env("PWD", p->env_nodes));
	if (!old_pwd)
		return (ft_error_int("cd: error", node));
	if (ft_setenv("OLDPWD", old_pwd, p->env_nodes) == -1)
		return (free(old_pwd), FALSE);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (free(old_pwd), ft_error_int("getcwd", node));
	if (ft_setenv("PWD", new_pwd, p->env_nodes) == -1)
		return (free(new_pwd), free(old_pwd), FALSE);
	return (free(new_pwd), free(old_pwd), TRUE);			
}
