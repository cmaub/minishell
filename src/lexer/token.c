/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:00:17 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 15:16:17 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fill_list_of_tokens(t_mega *mini, char *str)
{
	if (!expr(mini->L_input, &mini->tokens)
		|| !parser_has_reach_end(mini->L_input))
	{
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

int	add_new(t_token **list, t_token *new)
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

t_token	*create_new(t_lexer *input, int start, int end, int type)
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
	return (new);
}

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
