/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_types_nodes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:51:30 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 16:28:45 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fill_nodes_with_files(t_token **cur, t_parser **node, t_mega **mini)
{
	(*node)->file[(*mini)->f] = ft_strdup((*cur)->value);
	if (!(*node)->file[(*mini)->f])
		return (FALSE);
	(*node)->redir[(*mini)->f++] = (*cur)->type;
	return (TRUE);
}

int	fill_nodes_with_args(t_token **cur, t_parser **node, t_mega **mini)
{
	if ((*cur)->type == ARG && (*cur)->value != NULL)
	{
		while ((*cur) && (*cur)->value && !is_command((*cur)->value))
			(*cur) = (*cur)->next;
		(*node)->command[(*mini)->cmd] = ft_strdup((*cur)->value);
		if (!(*node)->command[(*mini)->cmd])
			return (FALSE);
		(*mini)->cmd++;
	}
	return (TRUE);
}

int	fill_nodes_with_(t_token *cur, t_parser *new_node, t_mega *mini)
{
	if (cur->type == REDIRECT_IN
		|| cur->type == REDIRECT_OUT
		|| cur->type == APPEND_OUT)
		fill_nodes_with_files(&cur, &new_node, &mini);
	else if (cur->type == HEREDOC && cur->value != NULL)
	{
		if (!fill_nodes_with_heredoc(&cur, &new_node, &mini))
			return (FALSE);
	}
	else if (cur->type == ARG && cur->value != NULL)
		fill_nodes_with_args(&cur, &new_node, &mini);
	return (TRUE);
}
