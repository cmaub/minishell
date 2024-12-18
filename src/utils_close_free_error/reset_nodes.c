/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:08:16 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 18:18:25 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_mini_null(t_mega *mini)
{
	mini->nodes = NULL;
	mini->p = NULL;
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
