/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:17:04 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/14 16:17:21 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.c"

void	free_array(char **array)
{
	int i;

	if (!array)
		return;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	reset_node(PARSER *node)
{
	if (node->infile)
	{
		free_array(node->infile);
		node->infile = NULL;
	}
	if (node->outfile)
	{
		free_array(node->outfile);
		node->outfile = NULL;
	}
	if (node->command)
	{
		free_array(node->command);
		node->command = NULL;
	}
	if (node->delimiter)
	{
		free_array(node->delimiter);
		node->delimiter = NULL;
	}
	if (node->redir_type_in)
	{
		free(node->redir_type_in);
		node->redir_type_in = NULL;
	}
	if (node->redir_type_out)
	{
		free(node->redir_type_out);
		node->redir_type_out = NULL;
	}
	if (node->fd_heredoc)
	{
		free(node->fd_heredoc);
		node->fd_heredoc = NULL;
	}

	node->nb_infile = 0;
	node->nb_outfile = 0;
	node->nb_command = 0;
	node->nb_heredoc = 0;
}

void free_tokens(t_token **tokens)
{
	t_token *current;
	t_token *next;

	if (!tokens || !*tokens)
		return;
	current = *tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*tokens = NULL;
}
