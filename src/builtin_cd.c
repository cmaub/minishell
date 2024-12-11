/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/11 17:50:53 by anvander         ###   ########.fr       */
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

void	create_new_var(t_env **node, char *str)
{
	t_env	*new;

	new = try_malloc(sizeof(t_env));
	if (!new)
		return ;
	new->var = ft_strdup(str);
	new->next = NULL; 
	if (!new->var)
		return ;
	ft_lstadd_env_back(node, new);
}

int	ft_setenv(char *dest, char *src, t_env **env_nodes)
{	
	t_env 	*temp;
	char	*dest_tmp;
	char	*new_var;
	

	temp = *env_nodes;
	dest_tmp = ft_strjoin(dest, "=");
	if (!dest_tmp)
		return (-1);
	while (temp && temp->next && temp->var)
	{
		if (ft_strncmp(temp->var, dest_tmp, ft_strlen(dest_tmp)) == 0)
			break ;
		temp = temp->next;
	}
	if (temp->var == NULL)
	{
		if (ft_strncmp(dest_tmp, src, 6) == 0)
		{
			new_var = ft_strjoin(dest_tmp, src);
			if (!temp)
				return (free(dest_tmp), -1);
			create_new_var(env_nodes, new_var);
			free(new_var);
		}
		else
			return (-1);
	}
	free(temp->var);
	temp->var = NULL;
	temp->var = ft_strjoin(dest_tmp, src);
	if (!temp->var)
		return (free(dest_tmp), -1);
	return (free(dest_tmp), 0);
}



int	ft_cd(char **cmd, t_pipex *p, PARSER *node)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*pwd_var;

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
	{
		new_pwd = getcwd(NULL, 0);
		pwd_var = ft_strjoin("PWD=", new_pwd);
		free(new_pwd);
		create_new_var(p->env_nodes, pwd_var);
		free(pwd_var);
	}
	// return(ft_error_int("cd: path not found", node));
	old_pwd = return_var_from_env("PWD", p->env_nodes);
	if (!old_pwd)
		return (ft_error_int("cd: error", node));
	if (ft_setenv("OLDPWD", old_pwd, p->env_nodes) == -1)
		return (free(old_pwd), FALSE);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (free(old_pwd), ft_error_int("cd: error", node));
	if (ft_setenv("PWD", new_pwd, p->env_nodes) == -1)
		return (free(new_pwd), free(old_pwd), FALSE);
	return (free(new_pwd), free(old_pwd), TRUE);			
}
