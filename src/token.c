/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:00:17 by anvander          #+#    #+#             */
/*   Updated: 2024/12/17 18:37:08 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ***** RETIRER ?

// void	replace_prev_token(t_token **list, t_token *new)
// {
// 	t_token	*current;

// 	if (!list || !new)
// 		return ;
// 	if (*list == NULL)
// 	{
// 		*list = new;
// 		return ;
// 	}
// 	else
// 	{
// 		current = *list;
// 		while (current->next != NULL)
// 			current = current->next;
// 		if (current->prev != NULL)
// 			current->prev->next = new;
// 		else
// 			*list = new;
// 		new->prev = current->prev;
// 		new->next = NULL;
// 		free(current->value);
// 		free(current);
// 	}
// }

int	fill_list_of_tokens(t_mega *mini, char *str)
{
	if (!expr(mini->L_input, &mini->tokens) || !t_parser_has_reach_end(mini->L_input))
	{
		// //dprintf(2, "str = %s\n", str);
		ft_putendl_fd("syntax error", 2);
		g_signal = 0;
		mini->exit_code = 2;
		free(str);
		free(mini->L_input);
		free_tokens(&mini->tokens);
		str = NULL;
		mini->L_input = NULL;
		return (FALSE);
	}
	free(mini->L_input);
	return (TRUE);
}

int	add_new_token(t_token **list, t_token *new)
{
	t_token	*current;

	if (!list || !new)
		return (FALSE);
	if (*list == NULL)
	{
		*list = new;
		return (TRUE);
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
	return (TRUE);
}

t_token	*create_new_token(t_lexer *input, int start, int end, int type)
{
	t_token	*new;
	int		len;

	if (!input || !input->data || start < 0 || end < 0 || start > end)
       	return (NULL);
	len = end - start;
	new = try_malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_substr(input->data, start, len);
	if (!new->value)
		return (free_tokens(&new), NULL);
	new->type = type;
	new->prev = NULL;
	new->next = NULL;
	return(new);
}

// ***** RETIRER ?

// int create_and_add_token(t_lexer *input, int start, int end, t_token **list, int type)
// {
//     t_token *new_node;

//     new_node = create_new_token(input, start, end, type);
//     if (!new_node)
//         return (FALSE);
//     add_new_token(list, new_node);
//     return (TRUE);
// }


void	print_tokens_list(t_token **list)
{
	int	i;

	i = 0;
	if (!*list)
	{
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

