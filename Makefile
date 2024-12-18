# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/22 17:11:30 by cmaubert          #+#    #+#              #
#    Updated: 2024/12/18 19:21:31 by cmaubert         ###   ########.fr        #
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
SRC_FILES			=		init/main_loop.c \
							init/create_t_env.c \
							init/init_structs.c \
							parsing/create_node_utils.c \
							parsing/create_node.c \
							parsing/expand_utils.c \
							parsing/expand.c \
							parsing/fill_types_nodes.c \
							parsing/heredoc.c \
							parsing/process_strings.c \
							parsing/withdraw_quotes.c \
							exec/choose_exec.c \
							exec/choose_process.c \
							exec/create_process.c \
							exec/exec_builtins.c \
							exec/exec_utils.c \
							exec/ft_wait.c \
							exec/redir.c \
							exec/signals.c \
							exec/path.c \
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
							builtins/echo.c \
							builtins/pwd.c \
							builtins/env.c \
							builtins/exit.c \
							builtins/exit_utils.c \
							builtins/cd.c \
							builtins/cd_utils.c \
							builtins/unset.c \
							builtins/export.c \
							builtins/export_utils.c \
							builtins/export_utils2.c \
							utils_close_free_error/chained_list_utils.c \
							utils_close_free_error/fds1.c \
							utils_close_free_error/fds2.c \
							utils_close_free_error/free1.c \
							utils_close_free_error/free2.c \
							utils_close_free_error/ft_error.c \
							utils_close_free_error/reset_nodes.c 
							
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
