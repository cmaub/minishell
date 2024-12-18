/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:17:04 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 18:23:09 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// **** RETIRER ?
// void	free_new_node(t_parser *new_node)
// {
// 	if (new_node)
//     {
// 		if ((new_node->file && new_node->nb_file > 0))
// 			free(new_node->file);
// 		if (new_node->command && new_node->nb_command > 0)
// 			free(new_node->command);
// 		if (new_node->delimiter && new_node->nb_heredoc > 0)
// 			free(new_node->delimiter);
// 		if (new_node->redir && new_node->nb_file > 0)
// 			free(new_node->redir);
// 		if (new_node->fd_heredoc && new_node->nb_heredoc > 0)
// 			free(new_node->fd_heredoc);
// 		free(new_node);
// 	}
// }

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

void	check_and_free_new_node(t_parser *new_node)
{
	if (!new_node)
		return ;
	if ((!new_node->file && new_node->nb_file > 0)
		|| (!new_node->command && new_node->nb_command > 0)
		|| (!new_node->delimiter && new_node->nb_heredoc > 0)
		|| (!new_node->redir && new_node->nb_file > 0)
		|| (!new_node->fd_heredoc && new_node->nb_heredoc > 0))
	{
		free(new_node->file);
		free(new_node->command);
		free(new_node->delimiter);
		free(new_node->redir);
		free(new_node->fd_heredoc);
		free(new_node);
		new_node = NULL;
	}
}

void	free_tokens(t_token **tokens)
{
	t_token	*next;

	if (!tokens || !(*tokens))
		return ;
	while (*tokens)
	{
		next = (*tokens)->next;
		if ((*tokens)->value)
		{
			free((*tokens)->value);
			(*tokens)->value = NULL;
		}
		free((*tokens));
		*tokens = next;
	}
}

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

void	close_heredoc(t_parser *current)
{
	int	i;

	i = 0;
	while (i < current->nb_heredoc)
	{
		s_clse(&current->fd_heredoc[i][0]);
		s_clse(&current->fd_heredoc[i][1]);
		i++;
	}
}

// UTILISEE ? 
void	close_all_heredoc(t_mega *mini)
{
	t_parser	*current;

	if (!mini || !mini->nodes)
		return ;
	current = mini->begin;
	while (current)
	{
		close_heredoc(current);
		current = current->next;
	}
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

void	reset_one_node(t_parser **node)
{
	if (!node)
		return ;
	if (!*node)
	{
		if (node)
		{
			free(node);
			node = NULL;
		}
		return ;
	}
	if ((*node)->file)
		free_array_and_close_fds((*node)->file);
	if ((*node)->command)
		free_array((*node)->command);
	if ((*node)->delimiter)
		free_array((*node)->delimiter);
	if ((*node)->redir)
		free((*node)->redir);
	if ((*node)->fd_heredoc)
		free_arr_i((*node)->fd_heredoc, (*node));
	if ((*node))
		free((*node));
	(*node) = NULL;
	node = NULL;
}

void	init_mini_null(t_mega *mini)
{
	mini->nodes = NULL;
	mini->p = NULL;
}

void	rst_nde_mini(t_mega *mini, t_parser **node)
{
	t_parser	*current;
	t_parser	*temp;

	reset_one_node(node);
	if (!mini || !mini->nodes)
		return ;
	current = mini->nodes;
	while (current)
	{
		temp = current->next;
		if (current->file)
			free_array_and_close_fds(current->file);
		if (current->command)
			free_array(current->command);
		if (current->delimiter)
			free_array(current->delimiter);
		if (current->redir)
			free(current->redir);
		if (current->nb_heredoc)
			free_arr_i(current->fd_heredoc, current);
		if (current)
			free(current);
		current = temp;
	}
	init_mini_null(mini);
}

void	rst_nde(t_parser **node)
{
	t_parser	*current;
	t_parser	*temp;

	if (!node | !*node)
		return ;
	current = *node;
	while (current)
	{
		temp = current->next;
		if (current->file)
			free_array_and_close_fds(current->file);
		if (current->command)
			free_array(current->command);
		if (current->delimiter)
			free_array(current->delimiter);
		if (current->redir)
			free(current->redir);
		if (current->fd_heredoc)
			free_arr_i(current->fd_heredoc, current);
		if (current)
			free(current);
		current = temp;
	}
	*node = NULL;
}
