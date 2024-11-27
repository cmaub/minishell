/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:32:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/27 18:46:18 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"
# include <stddef.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>

# define true 1
# define false 0

# define REDIRECT_IN 1 /* < */
# define REDIRECT_OUT 2 /* > */
# define APPEND_OUT 3 /* >> */
# define HEREDOC 4 /* << */
# define PIPEX 5 /* | */
# define COMMAND 6 // plus besoin ?
# define OPTION 7 /* - + taile de 3 */
# define ARGUMENT 8
# define FILENAME 9
# define TBD 10
# define DELIMITER 11

# define FALSE 0
# define TRUE 1

typedef struct PARSER
{
	char **file;
	char	**command; // n'exec que la premiere
	char	**delimiter;
	int		*redir_type;
	int		*fd_heredoc;
	int		cmd;
	int		f;
	int	nb_command;
	int	nb_file;
	int	nb_heredoc;
	struct PARSER	*next;
	int			exit_code;
} PARSER ;

typedef struct _LEXER {
    char    *data;
    size_t  len;
    size_t  head;
} LEXER;

typedef struct s_token
{
    char            *value;
    int             type;
    int             index;
    struct s_token  *prev;
    struct s_token  *next;
}t_token;

typedef struct s_pipex
{
	int	         	ac;
	char			**mini_env;
	int		 	nb_cmd;
	int		       i; // ceci est un compteur qui compte les pid
	int		       prev_fd;
	int			pipefd[2];
	int			exit;
	int			flag;
	pid_t	        	pid;
	pid_t	        	last_pid;
}	t_pipex;

typedef struct s_sig
{
	int	sigint;
	int	sigquit;
	pid_t	pid;
	
}t_sig;

int create_and_add_token(LEXER *input, int start, int end, t_token **list, int type);


/* MINISHELL */
char	*get_path_and_check(char **split_cmd, char **env);
char	**copy_env(char **envp);
char	*return_var_from_env(char *str, char **mini_env);

void	add_new_token(t_token **list, t_token *new);
void	print_tokens_list(t_token **list);
void	ft_error(char *str);
int	ft_error_int(char *str, PARSER *node);
void	check_open(int fd);
void	ft_init_struct(t_pipex *p, char **envp, PARSER *nodes);
void	safe_close(int fd);
void	get_lines(PARSER *nodes, int i, int d);
void	handle_output_redirection(PARSER **nodes, t_pipex *p, int fd_out);
void   ft_close_error(int *fd, t_pipex *p, char *str);
void	replace_prev_token(t_token **list, t_token *new);
void	handle_c_signal(int signum);
void	handle_c_signal_child(int signum);
void	check_signal_handler();



int	give_type_to_token(t_token *token);
// int	list_size(t_token *list);
int		ft_size_list(PARSER **nodes);
int	is_str(char *str);
int	simple_cmd(t_pipex *p, char *heredoc, PARSER *current, PARSER *nodes);
int	no_envp(char **tab);
int		env_var_exists(char **env, char *var);
int	handle_input(PARSER **nodes, t_pipex *p);
int	ft_here_doc(PARSER *nodes);
int	handle_input_redirection(t_pipex *p, PARSER *current, char *heredoc);
int	execute(PARSER *current, t_pipex *p);
int    ft_wait(pid_t last_pid, PARSER **nodes);

t_token	*create_new_token(LEXER *input, int start, int end, int type);
void	print_nodes_list(PARSER **nodes);
void	add_new_node(PARSER **nodes, PARSER *new_node);

/* BUILTINS */
int	ft_echo(char **cmd);
int	ft_pwd(PARSER *current);
int	ft_env(PARSER *current, char **mini_env);
int	ft_exit(char **cmd, t_pipex *p, PARSER *node);
int	ft_cd(char **cmd, t_pipex *p, PARSER *node);
char **ft_unset(PARSER *current, char **env);
int	ft_export(PARSER *current, char **env);
void	print_sorted_env(char **env);
int		env_var_exists(char **env, char *var);

/* ALLOC AND FREE */
void	ft_free_tab(char **tab);
void	free_new_node(PARSER *new_node);
void	reset_node(PARSER **node);

/* LEXER */
int PIPE(LEXER *input, t_token **list);
int R_ARROW(LEXER *input);
int L_ARROW(LEXER *input);
int SP(LEXER *input);
int S_QUOTE(LEXER *input);
int D_QUOTE(LEXER *input);
int DOLLAR(LEXER *input);
int QUESTION_M(LEXER *input);
int LOW_ALPHA(LEXER *input);
int UP_ALPA(LEXER *input);
int DIGIT(LEXER *input);
int SLASH(LEXER *input);
int MINUS(LEXER *input);
int DOT(LEXER *input);
int UNDERSCORE(LEXER *input);
int BACKSLASH(LEXER *input);
int ESPER(LEXER *input);
int SEMI_COL(LEXER *input);
int PRINTABLE_SQUOTE(LEXER *input);
int PRINTABLE_DQUOTE(LEXER *input);
int HAT(LEXER *input);
int	EQUAL(LEXER *input);
int	PLUS(LEXER *input);
int	STAR(LEXER *input);
int	TAB_H(LEXER *input);
int	TAB_V(LEXER *input);

int ows(LEXER *input);
int squote(LEXER *input);
int dquote(LEXER *input);
int arg(LEXER *input, t_token **list);
int redir(LEXER *input, t_token **list);
int command(LEXER *input, t_token **list);
int expr(LEXER *input, t_token **list);
int start(LEXER *input);

extern int	exit_status;
#endif
