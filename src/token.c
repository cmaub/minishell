/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:00:17 by anvander          #+#    #+#             */
/*   Updated: 2024/11/29 15:09:40 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Cette fonction permet de changer la nature du token identifie en supprimant celui qui vient d'etre ajoute s'il peut etre precise
*/
void	replace_prev_token(t_token **list, t_token *new)
{
	t_token	*current;

	if (!list || !new)
		return ;
	if (*list == NULL)
	{
		*list = new;
		return ;
	}
	else
	{
		current = *list;
		while (current->next != NULL)
			current = current->next;
		if (current->prev != NULL)
			current->prev->next = new;
		else
			*list = new;
		new->prev = current->prev;
		new->next = NULL;
		free(current->value);
		free(current);
	}
}

void	add_new_token(t_token **list, t_token *new)
{
	t_token	*current;

	if (!list || !new)
		return ;
	if (*list == NULL)
	{
		*list = new;
		return ;
	}
	else
	{
		current = *list;
		while (current->next != NULL)
		{
			current = current->next;
		}

		current->next = new;
		new->prev = current;
	}
}

t_token	*create_new_token(LEXER *input, int start, int end, int type)
{
	t_token	*new;
	int		len;

	if (!input || !input->data || start < 0 || end < 0 || start > end)
       	return NULL;
	len = end - start;
	new = try_malloc(sizeof(t_token));
	new->value = ft_substr(input->data, start, len);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->type = type;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

int create_and_add_token(LEXER *input, int start, int end, t_token **list, int type)
{
    t_token *new_node;

    new_node = create_new_token(input, start, end, type);
    if (!new_node)
        return (FALSE);
    add_new_token(list, new_node);
    return (TRUE);
}


void	print_tokens_list(t_token **list)
{
	int	i;

	i = 0;
	if (!*list)
	{
		// dprintf(2, "la liste tokens est nulle\n");
		return ;
	}
	while ((*list)->next)
	{
		if ((*list)->value)
           	printf("[%s] de type %d\n", (*list)->value, (*list)->type);
		(*list) = (*list)->next;
		i++;
	}
	printf("[%s] de type %d\n", (*list)->value, (*list)->type);
}

void	print_nodes_list(PARSER **nodes)
{
	int	f = 0;
	int	h = 0;
	int	d = 0;
	int	index = 0;
	PARSER	*tmp;

	tmp = (*nodes);
	if (!(*nodes))
		return ;
	while (index <= ft_size_list(nodes))
	{
		f = 0;
		h = 0;
		while (f < 30 && tmp->file && tmp->file[f] != NULL)
		{
			printf("tmp->file[%d] = %s, type = %d\n", f, tmp->file[f], tmp->redir_type[f]);
			if (tmp->delimiter && tmp->delimiter[d] != NULL)
			{
				printf("tmp->delimiter = %s\n", tmp->delimiter[d]);
				d++;
			}
			f++;
		}
		while (h < 30 && tmp->command && tmp->command[h] != NULL)
		{
			printf("tmp->command[%d] = %s\n", h, tmp->command[h]);
			h++;
		}
		if (!tmp->next)
			break;
		tmp = tmp->next;
		index++;
		printf("\n");
	}
	printf("\n");
}

void	add_new_node(PARSER **nodes, PARSER *new_node)
{
	PARSER	*current;

	if (!nodes || !new_node)
	{
		return ;
	}
	if (!(*nodes))
	{
		*nodes = new_node;
		return ;
	}
	else
	{
		current = *nodes;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}

}

