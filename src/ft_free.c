/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:17:04 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/13 14:29:40 by cmaubert         ###   ########.fr       */
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
			new_node = NULL;
	}
}

// **** RETIRER ?

// void	free_new_node(PARSER *new_node)
// {
// 	if (new_node)
//     {
// 		if ((new_node->file && new_node->nb_file > 0))
// 			free(new_node->file);
// 		if (new_node->command && new_node->nb_command > 0)
// 			free(new_node->command);
// 		if (new_node->delimiter && new_node->nb_heredoc > 0)
// 			free(new_node->delimiter);
// 		if (new_node->redir_type && new_node->nb_file > 0)
// 			free(new_node->redir_type);
// 		if (new_node->fd_heredoc && new_node->nb_heredoc > 0)
// 			free(new_node->fd_heredoc);
// 		free(new_node);
// 	}
// }

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
	int i;
	int		temp_file_fd;

	if (!array || !(*array))
		return;
	i = 0;
	while (array[i] && array[i] != NULL)
	{
		temp_file_fd = open(array[i], O_WRONLY);
		safe_close(&temp_file_fd);
		free(array[i]);
		i++;
	}
	free(array);
}



void	free_array_int(int **array, PARSER *current)
{
	int i;

	if (!array || !*array)
		return;
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

void	free_array(char **array)
{
	int i;

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

void	reset_one_node(PARSER **node)
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
	if ((*node)->redir_type)
		free((*node)->redir_type);
	if ((*node)->fd_heredoc)
		(close_heredoc((*node)), free_array_int((*node)->fd_heredoc, (*node)));
	if ((*node))
		free((*node));
	(*node) = NULL;
	node = NULL;
}

void	reset_node_mini(t_mega_struct *mini, PARSER **node)
{
	PARSER	*current;
	PARSER *temp;

	reset_one_node(node);
	current = mini->nodes;
	if (!mini->nodes)
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
			dprintf(2, "** %s, %d\n", __FILE__, __LINE__);
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
		}
		if (current->nb_heredoc)
		{
			close_heredoc(current);
			free_array_int(current->fd_heredoc, current);
		}
		if (current)
			free(current);
		current = temp;
	}
	// free(*node);
	// free(mini->nodes);
	mini->nodes = NULL;
	mini->p = NULL;
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
		}
		if (current->fd_heredoc)
		{
			close_heredoc(current);
			free_array_int(current->fd_heredoc, current);
		}
		if (current)
			free(current);
		current = temp;
	}
	*node = NULL;
}




