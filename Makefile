# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anvander <anvander@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/22 17:11:30 by cmaubert          #+#    #+#              #
#    Updated: 2024/12/18 15:46:26 by anvander         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# Compiler
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g

# Libft
LIBFT_PATH	= libft/
LIBFT_NAME	= libft.a
LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)

# Includes
INC			=	-I ./includes/\
				-I ./libft/\

# Sources
SRC_PATH	=	src/
SRC_FILES			=	minishell.c \
							nodes.c \
							utils.c \
							lexer/command.c \
							lexer/eat.c \
							lexer/expr.c \
							lexer/token.c \
							lexer/chars/alphanum.c \
							lexer/chars/arrows.c \
							lexer/chars/dots.c \
							lexer/chars/quotes1.c \
							lexer/chars/quotes2.c \
							lexer/chars/spaces.c \
							lexer/chars/specials.c \
							lexer/chars/maths_op.c \
							exec.c \
							get_next_line.c \
							path.c \
							builtin_echo.c \
							builtin_pwd.c \
							builtin_env.c \
							builtin_exit.c \
							builtin_cd.c \
							builtin_unset.c \
							builtin_export.c \
							ft_free.c

SRCS		= $(addprefix $(SRC_PATH), $(SRC_FILES))

# Objects
OBJ_PATH	= obj/
OBJ_FILES	= $(SRC_FILES:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ_FILES))

# Sous-dossiers pour les objets
DIRS		= $(sort $(dir $(OBJS)))

all: $(OBJ_PATH) $(LIBFT) $(NAME)

$(OBJ_PATH):
	@mkdir -p $(DIRS)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(LIBFT):
	@echo "Making libft..."
	@make -sC $(LIBFT_PATH)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(INC) -lreadline

clean:
	@echo "Removing .o object files..."
	@rm -rf $(OBJ_PATH)
	@make clean -C $(LIBFT_PATH)

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT_PATH)$(LIBFT_NAME)

re: fclean all

.PHONY: all re clean fclean