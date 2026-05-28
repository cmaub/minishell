/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:45:44 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 17:55:00 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_null_to_tab(t_parser *new_node, t_mega *mini)
{
	if (mini->f > 0)
		new_node->file[mini->f] = NULL;
	if (mini->d > 0)
		new_node->delimiter[mini->d] = NULL;
	if (mini->cmd > 0)
		new_node->command[mini->cmd] = NULL;
}

void	init_mini_counters(t_mega *mini)
{
		mini->f = 0;
		mini->d = 0;
		mini->cmd = 0;
}

int	update_value_in_node(t_token **cur, t_parser *node, t_mega *mini)
{
	char	*tmp;

	tmp = NULL;
	tmp = ft_strdup(((*cur))->value);
	if (!tmp)
		return (FALSE);
	free((*cur)->value);
	(*cur)->value = NULL;
	(*cur)->value = withdraw_quotes(node, mini, tmp);
	free(tmp);
	if (!(*cur)->value || (*cur)->value[0] == '\0')
	{
		return (FALSE);
	}
	return (TRUE);
}
