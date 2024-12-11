/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:52:18 by anvander          #+#    #+#             */
/*   Updated: 2024/12/11 12:00:32 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


#define DEBUG_S debug(input, (char*)__FUNCTION__)

void debug(LEXER* input, char* fname) {
	printf("%-30s > \033[30;1m%.*s\033[42;30;1m%c\033[0m\033[30;1m%s\033[0m\n", fname, (int)input->head, input->data, input->data[input->head], input->data + input->head + 1);
}

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
int	eatRange(LEXER *input, int start, int end)
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

int	PIPE(LEXER *input, t_token **list)
{
	// DEBUG_S;
	int	i;
	int	start;
	int	end;
	t_token	*new_node;

	// new_node = try_malloc(sizeof(t_token));
	i = 0;
	start = input->head;
	end = start;
	i = eat(input, '|');
	if (i == 0)
	{
		input->head = start;
		return (FALSE);
	}
	end = input->head;
	if (end > start)
	{
		new_node = create_new_token(input, start, end, PIPEX);
		add_new_token(list, new_node);
	}
	return (TRUE);
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

int	TAB_H(LEXER *input)
{
	return (eat(input, 9));
}

int	TAB_V(LEXER *input)
{
	return (eat(input, 11));
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

int	UNDERSCORE(LEXER *input)
{
	return (eat(input, '_'));
}

int	HAT(LEXER *input)
{
	return (eat(input, '^'));
}

int	MINUS(LEXER *input)
{
	return (eat(input, '-'));
}

int	DOT(LEXER *input)
{
	return (eat(input, '.'));
}

int	LOW_ALPHA(LEXER *input)
{
	int	i = 0;

	i = eatRange(input, 97, 122);
	if (!i)
		return (FALSE);
	return (TRUE);
}

int	UP_ALPHA(LEXER *input)
{
	int	i = 0;

	i = eatRange(input, 65, 90);
	if (!i)
		return (FALSE);
	return (TRUE);
}

int	DIGIT(LEXER *input)
{
	return (eatRange(input, 48, 57));
}

int	EQUAL(LEXER *input)
{
	return (eat(input, '='));
}

int	PLUS(LEXER *input)
{
	return (eat(input, '+'));
}

int	STAR(LEXER *input)
{
	return (eat(input, '*'));
}

int	DOTS(LEXER *input)
{
	return (eat(input, ':'));
}

int	PRINTABLE_SQUOTE(LEXER *input)
{
	int	i;
	int	j;
	int	save;

	save = input->head;

	i = eatRange(input, 32, 38);
	j = eatRange(input, 40, 126);
	if (i || j)
		return (TRUE);
	input->head = save;
	return (FALSE);
}

int	PRINTABLE_DQUOTE(LEXER *input)
{
	int	i;
	int	j;
	int	save;

	save = input->head;
	i = eatRange(input, 32, 33);
	j = eatRange(input, 35, 126);
	if (i || j)
		return (TRUE);
	input->head = save;
	return (FALSE);
}

int	ows(LEXER *input)
{
	while (SP(input) || TAB_H(input) || TAB_V(input))
	{}
	return (TRUE);
}

int ws(LEXER *input)
{
	int	save;

	save = input->head;
	if (SP(input) == FALSE || TAB_H(input) == FALSE || TAB_V(input) == FALSE)
	{
		input->head = save;
		return (FALSE);
	}
	ows(input);
	return (TRUE);
}

// entree comprise comme [""cat""] -> devrait peut-etre etre compris comme [""][cat][""] ?
int	dquote(LEXER *input)
{
	int	save;

	save = input->head;
	if(!DQUOTE(input))
	{
		input->head = save;
		return (FALSE);
	}
	while (PRINTABLE_DQUOTE(input))
	{
	}
	if (!DQUOTE(input))
	{
		input->head = save;
		return (FALSE);
	}
	return (TRUE);
}

int	squote(LEXER *input)
{
	int	save;

	save = input->head;
	if(!SQUOTE(input))
		return (FALSE);
	while (PRINTABLE_SQUOTE(input))
	{}
	if (!SQUOTE(input))
	{
		input->head = save;
		return (FALSE);
	}
	return (TRUE);
}

int	arg(LEXER *input)
{
	// DEBUG_S;
	int	i;
	int	start;

	i = 0;
	start = input->head;
	while  (LOW_ALPHA(input) || UP_ALPHA(input) || DOT(input) || squote(input) || DOLLAR(input) || QUESTION_M(input) || DOTS(input)
		|| dquote(input) || DIGIT(input) || HAT(input) || SLASH(input) || MINUS(input) || PLUS(input) || STAR(input) || UNDERSCORE(input) || EQUAL(input))
		i++;
	if (i < 1)
	{
		input->head = start;
		return (FALSE);
	}
	return (TRUE);
}

int redir(LEXER *input, t_token **list)
{
	// DEBUG_S;
	int save;
	int start;
	int type;

	save = input->head;
	if (R_ARROW(input)) {
		if (R_ARROW(input))
			type = APPEND_OUT;
		else
			type = REDIRECT_OUT;
	} else if (L_ARROW(input)) {
		if (L_ARROW(input))
			type = HEREDOC;
		else
			type = REDIRECT_IN;
	} else
		return (FALSE);
	ows(input);
	start = input->head;
	if (!arg(input)) {
		input->head = save;
		return (FALSE);
	}
	add_new_token(list, create_new_token(input, start, input->head, type));
	return (TRUE);
}


int command(LEXER *input, t_token **list)
{
	// DEBUG_S;
	int save;
	int start;
	int i;

	i = 0;
	save = input->head;
	while (TRUE) {
		if (!redir(input, list))
		{
			start = input->head;
			if (!arg(input))
			{
				break;
			}
			add_new_token(list, create_new_token(input, start, input->head, ARGUMENT));
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

int	expr(LEXER *input, t_token **list)
{
	// DEBUG_S;
	int	save;

	save = input->head;
	if (ows(input) && input->len == input->head)
		return (TRUE);
	if (!command(input, list))
	{
		//dprintf(2, "invalide car pas de cmd ni espaces\n");
		return (FALSE);
	}
	ows(input);
	while (TRUE)
	{
		if (input->len == input->head)
			break;
		// if ((PIPE(input, list) && command(input, list)) == FALSE)
		// {
		// 	if ((ws(input) && command(input, list)) == FALSE)
		// 	{
		// 		input->head = save;
		// 		return (FALSE);
		// 	}
		// }
		ows(input);
		if (!PIPE(input, list))
		{
			break;
		}
		ows(input);
		if (!command(input, list))
			return (FALSE);
	}
	return (TRUE);
}

