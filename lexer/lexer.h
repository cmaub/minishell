/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:43:26 by anvander          #+#    #+#             */
/*   Updated: 2024/10/22 16:17:37 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H


# define FALSE 0
# define TRUE 1

# include <stddef.h>
# include <stdio.h>
# include <string.h>

typedef struct _LEXER {
    char    *data; // notre string
    size_t  len;   // la length de la string
    int     head;  // notre tête de lecture
} LEXER;

int PIPE(LEXER * input);
int R_ARROW(LEXER * input);
int L_ARROW(LEXER * input);
int SP(LEXER * input);
int S_QUOTE(LEXER * input);
int D_QUOTE(LEXER * input);
int DOLLAR(LEXER * input);
int QUESTION_M(LEXER * input);
int LOW_ALPHA(LEXER * input);
int UP_ALPA(LEXER * input);
int DIGIT(LEXER * input);
int SLASH(LEXER * input);
int MINUS(LEXER * input);
int UNDERSCORE(LEXER * input);
int BACKSLASH(LEXER * input);
int ESPER(LEXER * input);
int SEMI_COL(LEXER * input);
int PRINTABLE(LEXER * input);
int HAT(LEXER * input);

int ows(LEXER * input);
int special_char(LEXER * input);
int squote(LEXER * input);
int dquote(LEXER * input);
int arg(LEXER * input);
int redir(LEXER * input);
int command(LEXER * input);
int expr(LEXER * input);
int start(LEXER * input);

#endif

