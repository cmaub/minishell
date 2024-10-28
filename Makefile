NAME = minishell

# Compiler
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -fPIC -g3

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
							utils.c \
							lexer.c \
							exec_pipe.c \
							exec_simple.c \
							get_next_line.c \
							path.c
SRCS		= $(addprefix $(SRC_PATH), $(SRC_FILES))

# Objects
OBJ_PATH	= obj/
OBJ			= $(SRC_FILES:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ))

all: $(OBJ_PATH) $(LIBFT) $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC) -lreadline

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
