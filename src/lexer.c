/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:52:18 by anvander          #+#    #+#             */
/*   Updated: 2024/10/25 17:56:12 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	int	i;
	int	start;
	int	end;
	t_token	*new_node;
	
	i = 0;
	start = input->head;
	end = start;
	i = eat(input, '|');
	if (!i)
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
	return (eat(input, '_'));
}

int	MINUS(LEXER *input)
{
	return (eat(input, '_'));
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

int	arg(LEXER *input, t_token **list)
{
	int	i;
	int	start;
	int	end;
	t_token	*new_node;

	i = 0;
	start = input->head;
	end = start;
	ows(input);
	while (LOW_ALPHA(input) || UP_ALPHA(input) || squote(input) 
		|| dquote(input) || DIGIT(input) || HAT(input) || SLASH(input) || MINUS(input) || UNDERSCORE(input))
		i++;
	if (i < 1)
	{
		input->head = start;
		return (FALSE);
	}
	end = input->head;
	if (end > start)
	{
		new_node = create_new_token(input, start, end, ARGUMENT);
		add_new_token(list, new_node);
	}
	return (TRUE);
}

/*
Sorry pour la treees longue fonction. C'est un travail pour notre 3e binome !
*/
int	redir(LEXER *input, t_token **list)
{
	int	start;
	int	end;
	t_token	*new_node;
	
	start = input->head;
	end = start; 
	if (R_ARROW(input))
	{
		end = input->head;
		if (R_ARROW(input))
		{
			end = input->head;
			if (end > start)
			{
				new_node = create_new_token(input, start, end, APPEND_OUT);
				add_new_token(list, new_node);
			}
			start = end;
			end = input->head;
			if (!arg(input, list))
			{
				input->head = start;
				return (FALSE);
			}
			start = end;
			end = input->head;
			if (end > start)
			{
				new_node = create_new_token(input, start, end, FILENAME);
				replace_prev_token(list, new_node);
			}
			return (TRUE);
		}
		// start = end;
		end = input->head;
		if (end > start)
		{
			new_node = create_new_token(input, start, end, REDIRECT_OUT);
			add_new_token(list, new_node);
		}
		ows(input);
		end = input->head;
		if (!arg(input, list))
		{
			input->head = start;
			return (FALSE);
		}
		start = end;
		end = input->head;
		if (end > start)
		{
			new_node = create_new_token(input, start, end, FILENAME);
			replace_prev_token(list, new_node);
		}
		return (TRUE);
	}
	else if (L_ARROW(input))
	{
		end =input->head;
		if (L_ARROW(input))
		{
			end = input->head;
			if (end > start)
			{
				new_node = create_new_token(input, start, end, HEREDOC);
				add_new_token(list, new_node);
			}
			ows(input);
			end = input->head;
			if (!arg(input, list))
			{
				input->head = start;
				return (FALSE);
			}
			start = end;
			end = input->head;
			if (end > start)
			{
				new_node = create_new_token(input, start, end, DELIMITER);
				replace_prev_token(list, new_node);
			}
			return (TRUE);
		}
		// start = end;
		end = input->head;
		if (end > start)
		{
			new_node = create_new_token(input, start, end, REDIRECT_IN);
			add_new_token(list, new_node);
		}
		ows(input);
		end = input->head;
		if (!arg(input, list))
		{
			input->head = start;
			return (FALSE);
		}
		start = end;
		end = input->head;
		if (end > start)
		{
			new_node = create_new_token(input, start, end, FILENAME);
			replace_prev_token(list, new_node);
		}
		return (TRUE);
	}
	input->head = start;
	return (FALSE);
}

int	ows(LEXER *input)
{
	while (SP(input)) 
	{}
	return (TRUE);
}

int ws(LEXER *input)
{
	int	save;

	save = input->head;
	if (SP(input) == FALSE)
	{
		input->head = save;
		return (FALSE);
	}
	ows(input);
	return (TRUE);
}

int	command(LEXER *input, t_token **list)
{
	int	start;
	
	start = input->head;
	ows(input);
	if (!arg(input, list) && ows(input) && !redir(input, list))
	{
		input->head = start;
		return (FALSE);
	}
	ows(input);
	while (arg(input, list) || redir(input, list))
		ows(input); 
	return (TRUE);
}

int    expr(LEXER *input, t_token **list)
{
	int	save;

	save = input->head;
	if (!command(input, list))
       	return (FALSE);
    	ows(input);
    	while (TRUE) 
	{
		if (input->len == input->head)
			break;
		if ((PIPE(input, list) && command(input, list)) == FALSE)
		{
			if ((ws(input) && command(input, list)) == FALSE)
			{
				input->head = save;
				return (FALSE);
			}
		}
	}
    printf("[expr OK]\n");
    print_tokens_list(list);
    return (TRUE);
}

// int	start(LEXER *input)
// {
// 	if (!expr(input))
// 		return (FALSE);
// 	return (TRUE);
// }

// int	main(int argc, char **argv)
// {
// 	LEXER Linput;
// 	Linput.data = argv[1];
// 	Linput.len = strlen(Linput.data);
// 	Linput.head = 0;
// 	(void)argv;
// 	if (argc > 0)
// 	{
// 		printf("result de start = %d\n", start(&Linput));
// 		printf("head est a %d\n", Linput.head);
// 		// printf("head est a %c\n", input.data[input.head + 1]);
// 	}
// 	return (0);
// }

