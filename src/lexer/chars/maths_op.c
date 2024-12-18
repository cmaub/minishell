/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:14:25 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 18:38:31 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_equal(t_lexer *input)
{
	return (eat(input, '='));
}

int	ft_min(t_lexer *input)
{
	return (eat(input, '-'));
}

int	ft_plus(t_lexer *input)
{
	return (eat(input, '+'));
}

int	ft_star(t_lexer *input)
{
	return (eat(input, '*'));
}

int	ft_slash(t_lexer *input)
{
	return (eat(input, '/'));
}
