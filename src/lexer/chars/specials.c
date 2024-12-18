/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specials.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:13:49 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 18:42:12 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dollar(t_lexer *input)
{
	return (eat(input, '$'));
}

int	ft_hat(t_lexer *input)
{
	return (eat(input, '^'));
}

int	ft_question(t_lexer *input)
{
	return (eat(input, '?'));
}

int	ft_undescore(t_lexer *input)
{
	return (eat(input, '_'));
}

