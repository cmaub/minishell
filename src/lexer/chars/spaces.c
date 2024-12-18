/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   space.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:06:12 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 15:06:49 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_space(t_lexer *input)
{
	return (eat(input, ' '));
}

int	ft_tab_h(t_lexer *input)
{
	return (eat(input, 9));
}

int	ft_tab_v(t_lexer *input)
{
	return (eat(input, 11));
}

int	ows(t_lexer *input)
{
	while (ft_space(input) || ft_tab_h(input) || ft_tab_v(input))
	{
	}
	return (TRUE);
}

int	ws(t_lexer *input)
{
	int	save;

	save = input->head;
	if (ft_space(input) == FALSE || ft_tab_h(input) == FALSE
		|| ft_tab_v(input) == FALSE)
	{
		input->head = save;
		return (FALSE);
	}
	ows(input);
	return (TRUE);
}
