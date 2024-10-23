# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    lexer.yacc                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/22 12:24:16 by anvander          #+#    #+#              #
#    Updated: 2024/10/23 17:40:22 by cmaubert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

// lexer /////////////////////////////
PIPE = '|'
R_ARROW = '>'
L_ARROW = '<'
SP = ' ' // space
H_TAB = 9
V_TAB = 11
S_QUOTE = 39
D_QUOTE = 34
DOLLAR = '$'
QUESTION_M = '?'
LOW_ALPHA = 'a-z'
UP_ALPHA = 'A-Z'
DIGIT = '0-9'
SLASH = '/'
BACKSLASH = '\'
ESPER = '&'
POINT_VIR = ';'
PRINTABLE = 32 - 126
HAT = '^'

// parser /////////////////////////////

ows = *SP

ws = 1*(SP)

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

expr = command  ows *(0*1[PIPE] 1*(command) )
                     / *( ws command)

start = [expr]

///  


// exemple //
ows = *SP

intpart = 1*(DIGIT)

// 5
// 5.5
// 32435.34
number = intpart [ DOT intpart ]

factor = number
       / LPAR ows expr ows RPAR



term = factor *( ows ( MUL / DIV ) ows factor )

// 65 - (1 + 4.5 * ((2 + 8) / 4.1) - 5) * (6 + 9)
expr = term *( ows ( ADD / SUB ) ows term )



start = [expr]

// 65 - (1 + 4.5 * ((2 + 8) / 4.1) - 5) * (6 + 9)
```