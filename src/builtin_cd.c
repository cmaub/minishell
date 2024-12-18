/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:43:49 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/13 19:05:06 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*si cd est appele sans ARGs -> fail (cf sujet)
si plus d'un ARG -> fail

chdir() permet de changer de repertoire. 
Retourne 0 en cas de succes et -1 si elle echoue

avant de changer de repertoire sauvegarder PWD dans OLDPWD
si le changement reussi, mettre a jour la nouvelle position

char *getcwd( char *buffer, size_t size );
permet de recuperer le chemin absolu du repertoire courant
- buffer : le bloc de mémoire dans lequel le chemin de travail 
courant vous sera retourné.
- size : la taille du bloc de mémoire passé en premier paramètre.
*/

int	handle_new_var(t_env **nodes, char *dest_tmp, char *src)
{
	char	*new_var;

	new_var = ft_strjoin(dest_tmp, src);
	if (!new_var)
		return (free(dest_tmp), -1);
	if (!create_new_var(nodes, new_var))
		return (free(new_var), FALSE);
	free(new_var);
	return (TRUE);
}

int	handle_existing_var(t_env *temp, char *dest_tmp, char *src)
{
	free(temp->var);
	temp->var = NULL;
	temp->var = ft_strjoin(dest_tmp, src);
	if (!temp->var)
		return (FALSE);
	return (TRUE);
}

int	ft_setenv(char *dest, char *src, t_env **env_n)
{	
	t_env	*temp;
	char	*dest_tmp;

	temp = *env_n;
	dest_tmp = ft_strjoin(dest, "=");
	if (!dest_tmp)
		return (FALSE);
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
			if (!handle_new_var(env_n, dest_tmp, src))
				return (free(dest_tmp), FALSE);
		}
	}
	else if (!temp->var || !handle_existing_var(temp, dest_tmp, src))
		return (free(dest_tmp), FALSE);
	return (free(dest_tmp), TRUE);
}

int	check_args_cd(char **cmd)
{
	if (!cmd[1])
		return (ft_putendl_fd("cd: no directory specified", 2), FALSE);
	if (cmd[2])
		return (ft_putendl_fd("cd: too many ARGs", 2), FALSE);
	else
	{
		if (chdir(cmd[1]) == -1)
			return (ft_putendl_fd("cd: error", 2), FALSE);
	}
	return (TRUE);
}

int	set_old_and_new_pwd(t_pipex *p, t_parser *node)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = return_var_from_env("PWD", p->env_n);
	if (!old_pwd)
		return (ft_error_int("cd: error", node));
	if (!ft_setenv("OLDPWD", old_pwd, p->env_n))
		return (free(old_pwd), FALSE);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (free(old_pwd), ft_error_int("cd: error", node));
	if (!ft_setenv("PWD", new_pwd, p->env_n))
		return (free(new_pwd), free(old_pwd), FALSE);
	return (free(new_pwd), free(old_pwd), TRUE);
}

int	ft_cd(char **cmd, t_pipex *p, t_parser *node)
{
	char	*new_pwd;
	char	*pwd_var;

	new_pwd = NULL;
	if (!check_args_cd(cmd))
	{
		node->exit_code = 1;
		return (FALSE);
	}
	if (!env_var_exists(p->env_n, "PWD"))
	{
		new_pwd = getcwd(NULL, 0);
		pwd_var = ft_strjoin("PWD=", new_pwd);
		if (!pwd_var)
			return (free(new_pwd), FALSE);
		(free(new_pwd), create_new_var(p->env_n, pwd_var), free(pwd_var));
	}
	if (!set_old_and_new_pwd(p, node))
		return (FALSE);
	if (new_pwd)
		free (new_pwd);
	return (TRUE);
}
