/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:17:04 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/04 16:05:30 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void check_and_free_new_node(PARSER *new_node)
{
	if (!new_node)
       	return;
	if ((!new_node->file && new_node->nb_file > 0) ||
        (!new_node->command && new_node->nb_command > 0) ||
        (!new_node->delimiter && new_node->nb_heredoc > 0) ||
        (!new_node->redir_type && new_node->nb_file > 0) ||
        (!new_node->fd_heredoc && new_node->nb_heredoc > 0))
	{
    		free(new_node->file);
    		free(new_node->command);
    		free(new_node->delimiter);
    		free(new_node->redir_type);
    		free(new_node->fd_heredoc);
    		free(new_node);
	}
}

void	free_new_node(PARSER *new_node)
{
	if (new_node)
    {
		if ((new_node->file && new_node->nb_file > 0))
			free(new_node->file);
		if (new_node->command && new_node->nb_command > 0)
			free(new_node->command);
		if (new_node->delimiter && new_node->nb_heredoc > 0)
			free(new_node->delimiter);
		if (new_node->redir_type && new_node->nb_file > 0)
			free(new_node->redir_type);
		if (new_node->fd_heredoc && new_node->nb_heredoc > 0)
			free(new_node->fd_heredoc);
		free(new_node);
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
			// printf("Freeing token with value: %s\n", (*tokens)->value);
			free((*tokens)->value);
			(*tokens)->value = NULL;
		}
		free((*tokens));
		*tokens = next;
	}
}

void	free_array_and_close_fds(char **array)
{
	int i;
	int		temp_file_fd;

	if (!array)
		return;
	i = 0;
	while (array[i])
	{
		temp_file_fd = open(array[i], O_WRONLY);
		safe_close(&temp_file_fd);
		free(array[i]);
		i++;
	}
	free(array);
}

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

void	close_heredoc(PARSER *current)
{
	int	i;

	i = 0;
	while(i < current->nb_heredoc)
	{
		safe_close(current->fd_heredoc[i]);
		i++;
	}
}

void	reset_node(PARSER **node)
{
	PARSER	*current;
	PARSER *temp;

	current = *node;
	if (!node || !(*node))
	{
		return ;
	}
	while (current)
	{
		temp = current->next;
		if (current->file)
		{
			free_array_and_close_fds(current->file);
			current->file = NULL;
		}
		if (current->command)
		{
			free_array(current->command);
			current->command = NULL;
		}
		if (current->delimiter)
		{
			free_array(current->delimiter);
			current->delimiter = NULL;
		}
		if (current->redir_type)
		{
			free(current->redir_type);
			current->redir_type = NULL;
		}
		if (current->fd_heredoc)
		{
			close_heredoc(current);
			free(current->fd_heredoc);
			current->fd_heredoc[2] = NULL;
			
		}
		free(current);
		current = temp;
	}
	*node = NULL;
}

