/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:42:29 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 14:50:42 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	calloc_tab_of_node(t_parser *node)
{
	int	i;

	i = 0;
	if (node->nb_file > 0)
	{
		node->file = try_malloc((node->nb_file + 1) * sizeof(char *));
		node->redir = try_malloc((node->nb_file) * sizeof(int));
	}
	if (node->nb_command > 0)
		node->command = try_malloc((node->nb_command + 3) * sizeof(char *));
	if (node->nb_heredoc > 0)
	{
		node->delimiter = try_malloc((node->nb_heredoc + 1) * sizeof(char *));
		node->fd_heredoc = try_malloc((node->nb_heredoc) * sizeof(int *));
		while (i < node->nb_heredoc)
		{
			node->fd_heredoc[i] = try_malloc(2 * sizeof(int));
			i++;
		}
	}
}
int	calculate_size_of_tab(t_token *cur, t_parser *node, t_mega *mini)
{
	char	*tmp;

	tmp = NULL;
	if (cur->type == REDIRECT_IN || cur->type == HEREDOC
		|| cur->type == REDIRECT_OUT || cur->type == APPEND_OUT)
	{
		if (cur->type == HEREDOC)
			node->nb_heredoc++;
		if (cur->type)
		{
			if (!update_value_in_node(&cur, node, mini))
				return (FALSE);
		}
		node->nb_file++;
	}
	else if (cur->type == ARGUMENT)
	{
		if (!update_value_in_node(&cur, node, mini))
			return (FALSE);
		node->nb_command++;
	}
	return (TRUE);
}

t_parser	*alloc_new_node(t_token *current, t_mega *mini, int exit_code)
{
	t_parser	*new_node;
	t_token		*cur;

	cur = current;
	new_node = try_malloc(sizeof(t_parser));
	if (!new_node)
		return (NULL);
	new_node->exit_code = exit_code;
	while (cur && cur->type != PIPEX)
	{
		calculate_size_of_tab(cur, new_node, mini);
		cur = cur->next;
	}
	calloc_tab_of_node(new_node);
	check_and_free_new_node(new_node);
	return (new_node);
}

void	add_new_node(t_mega *mini, t_parser **nodes, t_parser *new)
{
	t_parser	*current;

	if (!(nodes) || !new)
		return ;
	if (!(*(nodes)))
	{
		*(nodes) = new;
		mini->begin = *nodes;
		return ;
	}
	else
	{
		current = *(nodes);
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
}

int	create_nodes(t_mega *mini)
{
	t_token		*cur;
	t_parser	*new_node;

	cur = mini->tokens;
	while (cur)
	{
		init_mini_counters(mini);
		if (g_signal == 2)							
			mini->exit_code = g_signal + 128;
		g_signal = 0;
		new_node = alloc_new_node(cur, mini, mini->exit_code);
		if (!new_node)
			return (FALSE);
		while (cur && cur->type != PIPEX)
		{
			if (!fill_nodes_with_(cur, new_node, mini))
				return (FALSE);
			cur = cur->next;
		}
		add_null_to_tab(new_node, mini);
		add_new_node(mini, &mini->nodes, new_node);
		if (cur && cur->type == PIPEX)
			cur = cur->next;
	}
	return (TRUE);
}