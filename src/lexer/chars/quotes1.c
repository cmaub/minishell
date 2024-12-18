/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_quotes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:02:02 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 15:03:09 by anvander         ###   ########.fr       */
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
	int	save;

	save = input->head;
	i = eat_range(input, 32, 38);
	j = eat_range(input, 40, 126);
	if (i || j)
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
