/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:52:18 by anvander          #+#    #+#             */
/*   Updated: 2024/10/23 18:20:08 by cmaubert         ###   ########.fr       */
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
		printf("eat %c\n", input->data[input->head]);
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
		printf("eatRange %c\n", input->data[input->head]);
        input->head++;
        return TRUE;
    }
    return FALSE;
}

int	PIPE(LEXER *input)   
{ 
	int	i = 0;

	i = eat(input, '|');
	if (!i)
		return (FALSE);
	printf("pipe \n");
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
	int	i = 0;

	i = eat(input, '/');
	if (!i)
		return (FALSE);
	printf("slash \n");
	return (TRUE);
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
	int	i = 0;

	i = eat(input, '_');
	if (!i)
		return (FALSE);
	printf("underscore \n");
	return (TRUE);
}

// int	SEMI_COL(LEXER * input)
// {
// 	return (eat(input, ';'));
// }

int	HAT(LEXER *input)
{
	int	i = 0;

	i = eat(input, '^');
	if (!i)
		return (FALSE);
	printf("hat \n");
	return (TRUE);
}

int	MINUS(LEXER *input)
{
	int	i = 0;

	i = eat(input, '-');
	if (!i)
		return (FALSE);
	printf("minus \n");
	return (TRUE);
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
	int	i = 0;
	
	i = eatRange(input, 48, 57);
	if (!i)
		return (FALSE);
	printf("digit\n");
	return (TRUE);
}

int	PRINTABLE(LEXER *input) //exclure la meme quote que celle du debut de la chaine sous quote
{
	int	i;
	int	j;
	int	k;

	i = eatRange(input, 32, 33);
	j = eatRange(input, 35, 38);
	k = eatRange(input, 40, 126);
	if (i || j || k)
		return (TRUE);
	return (FALSE);
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
	printf("dquote avec input->data[input->head] = %c dans dquote\n", input->data[input->head]);
	return (TRUE);
}

int	squote(LEXER *input)
{
	//sauter les mm type de quote
	if(!SQUOTE(input))
		return (FALSE);
	while (PRINTABLE(input))
	{
	}
	if (!SQUOTE(input))
		return (FALSE);
	printf("squote avec input->data[input->head] = %c dans squote\n", input->data[input->head]);	
	return (TRUE);
}

int	arg(LEXER *input)
{
	int	i;

	i = 0;
	ows(input);
	while (LOW_ALPHA(input) || UP_ALPHA(input) || squote(input) 
		|| dquote(input) || DIGIT(input) || HAT(input) || SLASH(input) || MINUS(input) || UNDERSCORE(input))
	{
		i++;
	}
	if (i < 1)
		return (FALSE);
	printf("[arg]\n");
	return (TRUE);
}

// int	redir(LEXER *input)
// {
// 	// if (R_ARROW(input))
// 	// {
// 	// 	if(!arg(input))
// 	// 	{ 
// 	// 		if (!R_ARROW(input))
// 	// 			if (!arg(input))
// 	// 				return (FALSE);
// 	// 	}
// 	// }
// 	// else if (L_ARROW(input))
// 	// {
// 	// 	if(!arg(input))
// 	// 	{ 
// 	// 		if (!L_ARROW(input))
// 	// 			if (!arg(input))
// 	// 				return (FALSE);
// 	// 	}
// 	// }
// 	if ((R_ARROW(input) && arg(input)) || (L_ARROW(input) && arg(input))
// 		|| (R_ARROW(input) && R_ARROW(input) && arg(input))
// 		|| (L_ARROW(input) && L_ARROW(input) && arg(input)))
// 	{
// 		printf("[redir]\n");
// 		return (TRUE);
// 	}
// 	return (FALSE);
// }

int	redir(LEXER *input)
{
	int	save;
	
	save = input->head; 
	// Gère la redirection ">" ou ">>"
	if (R_ARROW(input))
	{
		// Vérifie si c'est ">>"
		if (R_ARROW(input))
		{
			if (!arg(input)) // Si pas d'argument après ">>", retour faux
			{
				printf("pas de [redir]\n");
				input->head = save;
				return (FALSE);
			}
			printf("[redir >>]\n");
			return (TRUE);
		}
		// Si c'est juste ">", on attend un argument après
		else if (!arg(input))
		{
			input->head = save;
			printf("pas de [redir]\n");
			return (FALSE);
		}
		printf("[redir >]\n");
		return (TRUE);
	}

	// Gère la redirection "<" ou "<<"
	else if (L_ARROW(input))
	{
		// Vérifie si c'est "<<"
		if (L_ARROW(input))
		{
			if (!arg(input)) // Si pas d'argument après "<<", retour faux
			{
				input->head = save;
				return (FALSE);
			}
			printf("[redir <<]\n");
			return (TRUE);
		}
		// Si c'est juste "<", on attend un argument après
		else if (!arg(input))
		{
			input->head = save;
			return (FALSE);
		}
		printf("[redir <]\n");
		return (TRUE);
	}
	input->head = save;
	return (FALSE);
}


int	ows(LEXER *input)
{
	while (SP(input)) 
	{
	}
	return (TRUE);
}

int ws(LEXER *input)
{
	if (SP(input) == FALSE)
		return (FALSE);
	ows(input);
	return (TRUE);
}

int	command(LEXER *input)
{
	int	save;
	
	save = input->head;
	ows(input);
	if (!arg(input) && !redir(input))
	{
		input->head = save;
		return (FALSE);
	}
	printf("betweeen command");
	while (arg(input) || redir(input)) 
	{
		ows(input); 
	}
	// printf("arg(input) = %d\n", arg(input));
	// printf("redir(input) = %d\n", redir(input));
	printf("[command]\n");
	return (TRUE);
}
int    expr(LEXER *input)
{
    if (!command(input))
        return (FALSE);
    ows(input);
    while (TRUE) {
		if (input->len == input->head)
			break;
		if ((PIPE(input) && command(input)) == FALSE)
		{
			if ((ws(input) && command(input)) == FALSE)
				return (FALSE);
		}
	}
    printf("[expr]\n");
    return (TRUE);
}
// int	expr(LEXER *input)
// {
// 	int	i = 0;
	
// 	if (!command(input))
// 		return (FALSE);
// 	ows(input);
// 	while (input->data[input->head] != '\0')
// 	{
// 		printf("input->data[input->head] = %c dans expr\n", input->data[input->head]);\
// 		if (!command(input))
// 		{
// 			if (PIPE(input))
// 			{
// 				printf("i = %d\n", i);
// 				i++;
// 				if (!command(input))
// 				{
// 					return (FALSE);
// 				}
// 			}
// 		}
// 		else if (!command(input))
// 		{
// 			printf("Line = %d\n", __LINE__);
// 			return (FALSE);
// 		}
// 	}
// 	printf("[expr]\n");
// 	return (TRUE);
// }

int	start(LEXER *input)
{
	if (!expr(input))
		return (FALSE);
	return (TRUE);
}

int	main(int argc, char **argv)
{
	LEXER input;
	input.data = argv[1];
	input.len = strlen(input.data);
	input.head = 0;
	(void)argv;
	if (argc > 0)
	{
		printf("result de start = %d\n", start(&input));
		printf("head est a %d\n", input.head);
		// printf("head est a %c\n", input.data[input.head + 1]);
	}
	return (0);
}

