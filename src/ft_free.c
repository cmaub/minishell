/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:17:04 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/25 16:06:56 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	reset_node(PARSER **node)
{
	PARSER	*current;
	PARSER *temp;
	
	current = *node;
	while (current)
	{
		temp = current->next;
		if (current->file)
		{
			free_array(current->file);
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
			free(current->fd_heredoc);
			current->fd_heredoc = NULL;
		}
		free(current);
		current = temp;
	}
	*node = NULL;

}

// void free_tokens(t_token **tokens)
// {
// 	t_token *current;
// 	t_token *next;

// 	if (!tokens || !*tokens)
// 		return;
// 	current = *tokens;
// 	while (current)
// 	{
// 		next = current->next;
// 		if (current->value)
// 			free(current->value);
// 		free(current);
// 		current = next;
// 	}
// 	*tokens = NULL;
// }
