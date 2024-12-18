/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dots.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:12:54 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 15:13:16 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dot(t_lexer *input)
{
	return (eat(input, '.'));
}

int	ft_dots(t_lexer *input)
{
	return (eat(input, ':'));
}
