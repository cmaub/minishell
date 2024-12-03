# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anvander < anvander@student.42.fr >        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/22 17:11:30 by cmaubert          #+#    #+#              #
#    Updated: 2024/12/03 10:39:37 by anvander         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# Compiler
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

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
							token.c \
							nodes.c \
							utils.c \
							lexer.c \
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
OBJ			= $(SRC_FILES:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ))


all: $(OBJ_PATH) $(LIBFT) $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

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
