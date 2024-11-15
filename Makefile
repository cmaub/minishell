NAME = minishell

# Compiler
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -g3

# Libft
LIBFT_PATH	= libft/
LIBFT_NAME	= libft.a
LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)

# Includes
INC			=	-I ./includes/\
				-I ./libft/\
				-I /opt/homebrew/opt/readline/include

# Sources
SRC_PATH	=	src/
SRC_FILES			=	minishell.c \
							token.c \
							utils.c \
							lexer.c \
							exec.c \
							get_next_line.c \
							path.c \
							builtin_echo.c \
							builtin_pwd.c \
							builtin_env.c \
							builtin_exit.c \
							builtin_unset.c \
							builtin_export.c
SRCS		= $(addprefix $(SRC_PATH), $(SRC_FILES))

# Objects
OBJ_PATH	= obj/
OBJ			= $(SRC_FILES:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ))

# Linker flags
LDFLAGS	= -L /opt/homebrew/opt/readline/lib -lreadline

all: $(OBJ_PATH) $(LIBFT) $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

$(LIBFT):
	@echo "Making libft..."
	@make -sC $(LIBFT_PATH)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(INC) $(LDFLAGS)

clean:
	@echo "Removing .o object files..."
	@rm -rf $(OBJ_PATH)
	@make clean -C $(LIBFT_PATH)

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT_PATH)$(LIBFT_NAME)

re: fclean all

.PHONY: all re clean fclean
