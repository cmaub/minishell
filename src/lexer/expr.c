/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:28:38 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 15:33:08 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
ows = *SP
ws = 1*(SP)

expr = command  ows *(0*1[PIPE] 1*(command) )
                     / *( ws command)
*/

int	ft_pipe(t_lexer *input, t_token **list)
{
	int		i;
	int		start;
	int		end;
	t_token	*new_node;

	i = 0;
	start = input->head;
	end = start;
	i = eat(input, '|');
	if (i == 0)
	{
		input->head = start;
		return (FALSE);
	}
	end = input->head;
	if (end > start)
	{
		new_node = create_new(input, start, end, PIPEX);
		if (!new_node)
			return (free_tokens(list), FALSE);
		if (!add_new(list, new_node))
			return (FALSE);
	}
	return (TRUE);
}

int	expr(t_lexer *input, t_token **list)
{
	int	save;

	save = input->head;
	if (ows(input) && input->len == input->head)
		return (TRUE);
	if (!command(input, list))
		return (FALSE);
	ows(input);
	while (TRUE)
	{
		if (input->len == input->head)
			break ;
		ows(input);
		if (!ft_pipe(input, list))
			break ;
		ows(input);
		if (!command(input, list))
			return (FALSE);
	}
	return (TRUE);
}
