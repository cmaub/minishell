/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:59:01 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 15:30:30 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
squote = SQUOTE *(PRINTABLE / H_TAB / V_TAB) SQUOTE
dquote = DQUOTE *(PRINTABLE / H_TAB / V_TAB) DQUOTE

arg = ows 1*(LOW_ALPHA 
       / UP_ALPHA
       / squote
       / dquote
       / DIGIT
       / HAT
       / SLASH
       / MINUS
       / UNDERSCORE)

redir = R_ARROW arg
       / L_ARROW arg
       / R_ARROW R_ARROW arg
       / L_ARROW L_ARROW arg

command = ows ( redir / arg )
*/

int	arg(t_lexer *input)
{
	int	i;
	int	start;

	i = 0;
	start = input->head;
	while (ft_l_alpha(input) || ft_u_alpha(input) || ft_dot(input)
		|| squote(input) || ft_dollar(input) || ft_question(input)
		|| ft_dots(input) || dquote(input) || ft_digit(input)
		|| ft_hat(input) || ft_slash(input) || ft_min(input) || ft_plus(input)
		|| ft_star(input) || ft_undescore(input) || ft_equal(input))
		i++;
	if (i < 1)
	{
		input->head = start;
		return (FALSE);
	}
	return (TRUE);
}

int	give_type_to_redir(t_lexer *input, int *type)
{
	if (ft_r_arrow(input))
	{
		if (ft_r_arrow(input))
			*type = APPEND_OUT;
		else
			*type = REDIRECT_OUT;
		return (TRUE);
	}
	else if (ft_l_arrow(input))
	{
		if (ft_l_arrow(input))
			*type = HEREDOC;
		else
			*type = REDIRECT_IN;
		return (TRUE);
	}
	else
		return (FALSE);
}

int	redir(t_lexer *input, t_token **list)
{
	int	save;
	int	start;
	int	type;

	save = input->head;
	if (!give_type_to_redir(input, &type))
		return (FALSE);
	ows(input);
	start = input->head;
	if (!arg(input))
	{
		input->head = save;
		return (FALSE);
	}
	if (!add_new(list, create_new(input, start, input->head, type)))
		return (FALSE);
	return (TRUE);
}

int	command(t_lexer *input, t_token **list)
{
	int	save;
	int	start;
	int	i;

	i = 0;
	save = input->head;
	while (TRUE)
	{
		if (!redir(input, list))
		{
			start = input->head;
			if (!arg(input))
				break ;
			if (!add_new(list, create_new(input, start, input->head, ARG)))
				return (FALSE);
		}
		ows(input);
		i++;
	}
	if (i < 1)
	{
		input->head = save;
		return (FALSE);
	}
	return (TRUE);
}
