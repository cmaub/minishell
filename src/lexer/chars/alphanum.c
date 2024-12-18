/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alphanum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:08:29 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 18:37:22 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// chars from 0(48) to 9(57)
int	ft_digit(t_lexer *input)
{
	return (eat_range(input, 48, 57));
}

// chars from a(97) to z(122)
int	ft_l_alpha(t_lexer *input)
{
	int	i;

	i = 0;
	i = eat_range(input, 97, 122);
	if (!i)
		return (FALSE);
	return (TRUE);
}

// chars from A(65) to Z(90)
int	ft_u_alpha(t_lexer *input)
{
	int	i;

	i = 0;
	i = eat_range(input, 65, 90);
	if (!i)
		return (FALSE);
	return (TRUE);
}

