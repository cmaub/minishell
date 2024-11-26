/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:17:04 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/26 18:20:00 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		safe_close(temp_file_fd);
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
		return ;
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
			current->fd_heredoc = NULL;
			
		}
		free(current);
		current = temp;
	}
	*node = NULL;
}

