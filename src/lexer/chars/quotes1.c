/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:02:02 by anvander          #+#    #+#             */
/*   Updated: 2024/12/19 18:42:51 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_squote(t_lexer *input)
{
	return (eat(input, 39));
}

int	print_ft_squote(t_lexer *input)
{
	int	i;
	int	j;
	int	k;
	int	save;

	save = input->head;
	i = eat_range(input, 32, 38);
	j = eat_range(input, 40, 126);
	k = eat_range(input, 9, 11);
	if (i || j || k)
		return (TRUE);
	input->head = save;
	return (FALSE);
}

int	squote(t_lexer *input)
{
	int	save;

	save = input->head;
	if (!ft_squote(input))
		return (FALSE);
	while (print_ft_squote(input))
	{
	}
	if (!ft_squote(input))
	{
		input->head = save;
		return (FALSE);
	}
	return (TRUE);
}
