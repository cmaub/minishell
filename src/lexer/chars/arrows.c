/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrows.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:07:46 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 18:37:41 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_l_arrow(t_lexer *input)
{
	return (eat(input, '<'));
}

int	ft_r_arrow(t_lexer *input)
{
	return (eat(input, '>'));
}
