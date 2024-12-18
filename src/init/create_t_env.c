/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_t_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:04:44 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 18:56:20 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_new_var(t_env **mini_env, t_env *new_var)
{
	if (!new_var || !mini_env)
		return ;
	ft_lstadd_env_back(mini_env, new_var);
}

int	create_full_env(t_env **mini_env, char **env)
{
	t_env	*new_env;
	int		i;

	new_env = NULL;
	i = 0;
	while (env[i] && env[i] != NULL)
	{
		new_env = try_malloc(sizeof(t_env));
		if (!new_env)
			return (free_env(mini_env), FALSE);
		new_env->var = ft_strdup(env[i]);
		if (!new_env->var)
			return (free_env(mini_env), free_env(&new_env), FALSE);
		new_env->next = NULL;
		add_new_var(mini_env, new_env);
		i++;
	}
	return (TRUE);
}

int	create_minimalist_env(t_env **mini_env)
{
	t_env	*new_var_pwd;
	t_env	*new_var_shlvl;
	t_env	*new_var_;

	new_var_pwd = try_malloc(sizeof(t_env));
	if (!new_var_pwd)
		return (free_env(mini_env), FALSE);
	new_var_pwd->var = ft_strjoin("PWD=", getcwd(NULL, 0));
	if (!new_var_pwd->var)
		return (free_env(mini_env), free_env(&new_var_pwd), FALSE);
	add_new_var(mini_env, new_var_pwd);
	new_var_shlvl = try_malloc(sizeof(t_env));
	if (!new_var_shlvl)
		return (free_env(mini_env), FALSE);
	new_var_shlvl->var = ft_strdup("SHLVL=1");
	if (!new_var_shlvl->var)
		return (free_env(mini_env), free_env(&new_var_shlvl), FALSE);
	add_new_var(mini_env, new_var_shlvl);
	new_var_ = try_malloc(sizeof(t_env));
	if (!new_var_)
		return (free_env(mini_env), FALSE);
	new_var_->var = ft_strdup("_=./minishell");
	if (!new_var_->var)
		return (free_env(mini_env), free_env(&new_var_), FALSE);
	return (add_new_var(mini_env, new_var_), TRUE);
}

t_env	**copy_env_list(t_env **mini_env, char **env)
{
	mini_env = try_malloc(sizeof(t_env));
	if (!mini_env)
		return (NULL);
	if (!env || !(*env))
	{
		if (!create_minimalist_env(mini_env))
			return (NULL);
	}
	else
	{
		if (!create_full_env(mini_env, env))
			return (NULL);
	}
	return (mini_env);
}
