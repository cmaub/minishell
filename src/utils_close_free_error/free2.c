/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:11:34 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 18:12:52 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pipex(t_pipex **p)
{
	if (!p || !*p)
		return ;
	free(*p);
	*p = NULL;
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
