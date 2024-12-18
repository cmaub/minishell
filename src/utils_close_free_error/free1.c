/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:17:04 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 18:52:21 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array_and_close_fds(char **array)
{
	int	i;
	int	temp_file_fd;

	if (!array || !(*array))
		return ;
	i = 0;
	while (array[i] && array[i] != NULL)
	{
		temp_file_fd = open(array[i], O_WRONLY);
		s_clse(&temp_file_fd);
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

void	free_arr_i(int **array, t_parser *current)
{
	int	i;

	if (!array || !*array)
		return ;
	i = 0;
	while (i < current->nb_heredoc)
	{
		if (array[i])
		{
			free(array[i]);
		}
		i++;
	}
	free(array);
	array = NULL;
}

void	free_exec_input(t_mega *mini)
{
	free_tokens(&mini->tokens);
	mini->p = try_malloc(sizeof(t_pipex));
	if (!mini->p)
	{
	}
	else
	{
		init_pipex(mini->p, mini->chained_env, mini->nodes);
		create_process(&mini->nodes, mini->p, mini);
		if (mini->nodes)
			mini->exit_code = mini->nodes->exit_code;
		free_pipex(&mini->p);
	}
}

void	free_env(t_env **mini_env)
{
	t_env	*next;

	if (!mini_env || !(*mini_env))
		return ;
	while (*mini_env)
	{
		next = (*mini_env)->next;
		if ((*mini_env)->var)
		{
			free((*mini_env)->var);
			(*mini_env)->var = NULL;
		}
		free((*mini_env));
		*mini_env = next;
	}
	free(mini_env);
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	if (!*array)
	{
		if (array)
		{
			free(array);
			array = NULL;
		}
		return ;
	}
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}
