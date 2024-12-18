/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_quotes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:03:47 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 15:04:44 by anvander         ###   ########.fr       */
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
	int	save;

	save = input->head;
	i = eat_range(input, 32, 33);
	j = eat_range(input, 35, 126);
	if (i || j)
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
