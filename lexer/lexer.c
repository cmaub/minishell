/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:52:18 by anvander          #+#    #+#             */
/*   Updated: 2024/10/22 17:15:24 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
eat est la seul fonction qui a le droit de lire notre chaine
son but est de verifier si le char donné en param est bien le char pointé par notre tete de lecture
si oui: on avance la head sur le prochain char et on return TRUE
si non ou si la head est deja arrivé a la fin de la chaine: on return FALSE
*/
int	eat(LEXER *input, char c) 
{
    if (!(input->head < input->len))
        return FALSE;
    if (input->data[input->head] == c) 
    {
	 printf("input->data[input->head] = %c\n", input->data[input->head]);
        input->head++;
        return TRUE;
    }
    return FALSE;
}

/*
identique a 'eat', mais check une range de char (utile pour verifier si le
char pointé par head est entre 'a' et 'z' par exemple :p
(start et end inclus)
*/
int	eatRange(LEXER *input, char start, char end) 
{
    if (!(input->head < input->len))
        return FALSE;
    if (input->data[input->head] >= start && input->data[input->head] <= end) 
    {
        input->head++;
        return TRUE;
    }
    return FALSE;
}

int	PIPE(LEXER *input)   
{ 
	return (eat(input, '|')); 
}

int	R_ARROW(LEXER *input)
{
	return (eat(input, '>'));
}

int	L_ARROW(LEXER *input)
{
	return (eat(input, '<'));
}

int	SP(LEXER *input)
{
	return (eat(input, ' '));
}

int	SQUOTE(LEXER *input)
{
	return (eat(input, 39));
}

int	DQUOTE(LEXER * input)
{
	return (eat(input, 34));
}

int	DOLLAR(LEXER *input)
{
	return (eat(input, '$'));
}

int	QUESTION_M(LEXER *input)
{
	return (eat(input, '?'));
}

int	SLASH(LEXER *input)
{
	return (eat(input, '/'));
}

// int	BACKSLASH(LEXER * input)
// {
// 	return (eat(input, 92));
// }

// int	ESPER(LEXER * input)
// {
// 	return (eat(input, '&'));
// }

int	UNDERSCORE(LEXER *input)
{
	return (eat(input, '_'));
}

// int	SEMI_COL(LEXER * input)
// {
// 	return (eat(input, ';'));
// }

int	HAT(LEXER *input)
{
	return (eat(input, '^'));
}

int	MINUS(LEXER *input)
{
	return (eat(input, '-'));
}

int	LOW_ALPHA(LEXER *input)
{
	return (eatRange(input, 'a', 'z'));
}

int	UP_ALPHA(LEXER *input)
{
	return (eatRange(input, 'A', 'Z'));
}

int	DIGIT(LEXER *input)
{
	return (eatRange(input, '0', '9'));
}

int	PRINTABLE(LEXER *input)
{
	return (eatRange(input, 32, 126));
}

int	dquote(LEXER *input)
{
	if(!DQUOTE(input))
		return (FALSE);
	while (PRINTABLE(input))
	{
	}
	if (!DQUOTE(input))
		return (FALSE);
	return (TRUE);
}

int	squote(LEXER *input)
{
	if(!SQUOTE(input))
		return (FALSE);
	while (PRINTABLE(input))
	{
	}
	if (!SQUOTE(input))
		return (FALSE);
	return (TRUE);
}

int	arg(LEXER *input)
{
	int	i;

	i = 0;
	while (LOW_ALPHA(input) || UP_ALPHA(input) || squote(input) 
		|| dquote(input) || DIGIT(input) || HAT(input) || SLASH(input) || MINUS(input) || UNDERSCORE(input))
	{
		printf("input->data[input->head] = %c\n", input->data[input->head]);
		i++;
	}
	if (i < 1)
		return (FALSE);
	return (TRUE);
}

int	redir(LEXER *input)
{
	if ((R_ARROW(input) && arg(input)) || (L_ARROW(input) && arg(input))
		|| (R_ARROW(input) && R_ARROW(input) && arg(input))
		|| (L_ARROW(input) && L_ARROW(input) && arg(input)))
	{
		printf("[redir]\n");
		return (TRUE);
	}
	return (FALSE);
}

int	ows(LEXER *input)
{
	while (SP(input)) 
	{
		// if (!SP(input))
		// 	return (FALSE);
	}
	printf("[space]\n");
	return (TRUE);
}

int	command(LEXER *input)
{
	if (!ows(input))
		return (FALSE);
	if (!redir(input) && !ows(input) && !arg(input))
		return (FALSE);
	printf("[command]\n");
	return (TRUE);
}

int	expr(LEXER *input)
{
	if (!command(input))
		return (FALSE);
	while (command(input) ||  PIPE(input))
	{
		if (input->data[input->head] == '\0')
			break;
	}
	printf("[expr]\n");
	return (TRUE);
}

int	start(LEXER *input)
{
	if (!expr(input))
		return (FALSE);
	return (TRUE);
}

int	main(int argc, char **argv)
{
	LEXER input;
	input.data = "<file&";
	input.len = strlen(input.data);
	input.head = 0;
	(void)argv;
	if (argc > 0)
	{
		start(&input);
		// printf("result de start = %d\n", );
		// printf("head est a %d\n", input.head);
		// printf("head est a %c\n", input.data[input.head + 1]);
	}
	return (0);
}

