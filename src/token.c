/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:00:17 by anvander          #+#    #+#             */
/*   Updated: 2024/10/16 16:04:18 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_new_token(t_token **list, t_token *new)
{
	t_token	*current;
	
	if (!list || !new)
		return ;
	if (*list == NULL)
	{
		*list = new;
		new->prev = NULL;
		return ;
	}
	else
	{
		current = *list;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
		new->prev = current;
	}
}
	
t_token	*create_new_token(char *input)
{
	t_token	*new;

	new = malloc (sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = input;
	new->type = 0;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

void	print_tokens_list(t_token *list)
{
	while (list != NULL)
	{
		printf("[%s] de type %d\n", list->value, list->type);
		list = list->next;
	}
}


