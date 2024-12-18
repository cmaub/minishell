/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:04:02 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 18:28:38 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_list_in_str(t_env **env_n)
{
	t_env	*temp;
	char	**str_env;
	int		i;

	if (!env_n || !*env_n)
		return (NULL);
	temp = *env_n;
	str_env = try_malloc(sizeof(char *) * (lstsize_t_env(env_n) + 1));
	if (!str_env)
		return (NULL);
	i = 0;
	while (temp)
	{
		str_env[i] = ft_strdup(temp->var);
		if (!str_env[i])
			return (NULL);
		i++;
		if (temp->next)
			temp = temp->next;
		else
			break ;
	}
	str_env[i] = NULL;
	return (str_env);
}

char	**copy_tab(char **tab)
{
	char	**new_tab;
	int		i;

	i = 0;
	if (!tab)
		return (NULL);
	while (tab[i])
		i++;
	new_tab = try_malloc((i + 1) * sizeof(char *));
	if (!new_tab)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		new_tab[i] = ft_strdup(tab[i]);
		if (!new_tab[i])
		{
			free_array(new_tab);
			return (NULL);
		}
		i++;
	}
	new_tab[i] = NULL;
	return (new_tab);
}

int	is_command(char *cmd)
{
	if (!cmd || cmd[0] == '\0' || ft_is_only_spaces(cmd))
	{
		return (FALSE);
	}
	return (TRUE);
}

int	find_path(char **env)
{
	int	i;

	i = 0 ;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}
