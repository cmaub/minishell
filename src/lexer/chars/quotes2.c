/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:03:47 by anvander          #+#    #+#             */
/*   Updated: 2024/12/19 18:48:12 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dquote(t_lexer *input)
{
	return (eat(input, 34));
}

int	print_ft_dquote(t_lexer *input)
{
	int	i;
	int	j;
	int	k;
	int	save;

	save = input->head;
	i = eat_range(input, 32, 33);
	j = eat_range(input, 35, 126);
	k = eat_range(input, 9, 11);
	if (i || j || k)
		return (TRUE);
	input->head = save;
	return (FALSE);
}

int	dquote(t_lexer *input)
{
	int	save;

	save = input->head;
	if (!ft_dquote(input))
	{
		input->head = save;
		return (FALSE);
	}
	while (print_ft_dquote(input))
	{
	}
	if (!ft_dquote(input))
	{
		input->head = save;
		return (FALSE);
	}
	return (TRUE);
}
