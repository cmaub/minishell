/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:00:17 by anvander          #+#    #+#             */
/*   Updated: 2024/10/30 15:29:32 by cmaubert         ###   ########.fr       */
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

	len = end - start;
	new = malloc (sizeof(t_token));
	if (!new)
		return (NULL);
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
	if (!*list)
		return ;
	while ((*list)->next)
	{
		if ((*list)->value)
        	{
         	   	printf("[%s] de type %d\n", (*list)->value, (*list)->type);
			(*list) = (*list)->next;
		}
	}
	printf("[%s] de type %d\n", (*list)->value, (*list)->type);
}

void	print_nodes_list(PARSER *nodes)
{
	int	i = 0;
	int	j = 0;
	int h = 0;
	int	index = 0;
	PARSER	*tmp;
	
	tmp = nodes;
	if (!nodes)
		return ;
	// while (tmp != NULL)
	// {
	// 	if (!tmp->next)
	// 		break;
	// 	tmp = tmp->next;
	// 	count++;
	// }
	while (tmp != NULL)
	{
		i = 0;
		j = 0;
		h = 0;
		printf("index = %d, ligne %d\n", index, __LINE__);
		while (i < 10 && tmp->infile && tmp->infile[i] != NULL)
		{
			printf("nodes->infile[%d] = %s, type = %d\n", i, tmp->infile[i], tmp->redir_type_in[i]);
			i++;
		}
		while (j < 10 && tmp->outfile && tmp->outfile[j] != NULL)
		{
			printf("tmp->outfile[%d] = %s, type = %d\n", j, tmp->outfile[j], tmp->redir_type_out[j]);
			j++;
		}
		while (h < 10 && tmp->command && tmp->command[h] != NULL)
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
	// dprintf(2, "line %d, file %s\n", __LINE__, __FILE__);
}

void	add_new_node(PARSER *nodes, PARSER *new_node)
{
	PARSER	*current;
	
	if (!nodes || !new_node)
		return ;
	if (nodes == NULL)
	{
		nodes = new_node;
		return ;
	}
	else
	{
		current = nodes;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

