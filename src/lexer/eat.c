/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:57:01 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 15:22:39 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
eat is the only function allowed to read the string.
Purpose: check if the char giver as a parameter matches
the char currently pointed by the reading head.
- YES: move the head to the next char and return TRUE
- NO: or the head is already at the end, return FALSE
*/
int	eat(t_lexer *input, char c)
{
	if (!(input->head < input->len))
		return (FALSE);
	if (input->data[input->head] == c)
	{
		input->head++;
		return (TRUE);
	}
	return (FALSE);
}

/*
Similar to 'eat', but checks a range of characters
(start and end inclusive)
*/
int	eat_range(t_lexer *input, int start, int end)
{
	if (!(input->head < input->len))
		return (FALSE);
	if (input->data[input->head] >= start && input->data[input->head] <= end)
	{
		input->head++;
		return (TRUE);
	}
	return (FALSE);
}
