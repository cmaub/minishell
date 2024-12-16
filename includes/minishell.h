/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:32:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/16 16:54:44 by cmaubert         ###   ########.fr       */
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


// typedef struct s_exec
// {
// 	char	*path;
// 	char	**tmp_cmd;
// 	char	**tmp_minienv;
// 	char	**str_env;
// } t_exec;

typedef struct PARSER
{
	char **file;
	char	**command; // n'exec que la premiere
	char	**delimiter;
	int		*redir;
	int		cmd;
	int		f;
	int	nb_command;
	int	nb_file;
	int	nb_heredoc;
	struct PARSER	*next;
	int			exit_code;
	int	**fd_heredoc;
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

typedef struct s_env
{
	char	*var;
	struct s_env *prev;
	struct s_env *next;
}t_env;

typedef struct s_pipex
{
	char			**mini_env;
	int		 	nb_cmd;
	int		       i; // ceci est un compteur qui compte les pid
	int				d; //compteur pour le pipe heredoc
	int		       prev_fd;
	int			pipefd[2];
	int			exit;
	int			flag;
	pid_t	        	pid;
	pid_t	        	last_pid;
	t_env			**env_nodes;
}	t_pipex;

typedef struct s_sig
{
	int	sigint;
	int	sigquit;
	pid_t	pid;
	
}t_sig;

typedef struct s_mega_struct
{
	PARSER *nodes;
	t_token *tokens;
	LEXER	*L_input;
	t_env	**chained_env;
	t_pipex *p;
	int	exit_code;
	char	*str;
} t_mega_struct;

// ENV
int	lstsize_t_env(t_env **lst);
void	ft_lstadd_env_back(t_env **lst, t_env *new);
void	add_new_var(t_env **mini_env, t_env *new_var);
void	print_t_env(t_env **mini_env);
int	count_env_var(char **list);
int	env_var_exists(t_env **env_nodes, char *var);
char	*return_var_from_env(char *str, t_env **chained_env);
t_env	**copy_t_env(t_env **env);
char	**find_path_line(char **env);

/* MINISHELL */
char	*get_path_and_check(char **split_cmd, char **env);
char	**copy_tab(char **envp);
int	ft_error_int(char *str, PARSER *node);
void	ft_error_exit(char *str, int exit_c);
void	ft_close_error_no_exit(int *fd, t_pipex *p, char *str);
void	check_open(int fd);
void	ft_init_struct(t_pipex *p, t_env **chained_env, PARSER *nodes);
void	safe_close(int *fd);
// int		handle_output_redirection(PARSER **nodes, t_pipex *p, int fd_out);
int		handle_output_redirection(PARSER **nodes, t_pipex *p, int *flag_output);
void	handle_c_signal(int signum);
void	handle_c_signal_wait(int signum);
// void	handle_c_signal_child(int signum);
// void	check_signal_handler();

/* TOKENS */
int	fill_list_of_tokens(t_mega_struct *mini, char *str);
int	no_envp(char **tab);
int	handle_input(PARSER **nodes, t_pipex *p);
int	ft_here_doc(PARSER *nodes);
int	execute(PARSER *current, t_pipex *p);
int    ft_wait(pid_t last_pid, PARSER **nodes);
int	add_new_token(t_token **list, t_token *new);
void	print_tokens_list(t_token **list);
t_token	*create_new_token(LEXER *input, int start, int end, int type);
// int create_and_add_token(LEXER *input, int start, int end, t_token **list, int type);

/* NODES */
int	create_nodes(t_mega_struct *mini);
void	print_nodes_list(PARSER **nodes);
void	add_new_node(PARSER **nodes, PARSER *new_node);
int	ft_size_list(PARSER **nodes);
int	is_command(char *cmd);

/* BUILTINS */
int	ft_echo(char **cmd);
int	ft_pwd(PARSER *current);
int	ft_env(PARSER *current, t_env **env);
int	ft_exit(char **cmd, t_pipex *p, PARSER *node, int *cpy_stdin, int *cpy_stdout);
int	ft_cd(char **cmd, t_pipex *p, PARSER *node);
int	create_new_var(t_env **node, char *str);
int	env_var_exists(t_env **env_nodes, char *var);
void	print_sorted_env(t_env **env_nodes);
t_env	**ft_unset(PARSER *current, t_env **env_nodes);
t_env	**ft_export(PARSER *current, t_env **env_nodes);

/* ALLOC AND FREE */
void	ft_free_tab(char **tab);
char	**copy_tab_free(char **envp);
void	free_new_node(PARSER *new_node);
void	reset_node(PARSER **node);
void	reset_node_mini(t_mega_struct *mini, PARSER **node);
void	free_tokens(t_token **tokens);
void	check_and_free_new_node(PARSER *new_node);
void	free_t_env(t_env **mini_env);
void	free_array_int(int **array, PARSER *current);
void	reset_one_node(PARSER **node);
void   close_error_and_free(int *fd, t_pipex *p, PARSER **nodes, char *str, int exit_c);
void	free_array(char **array);
void	free_pipex(t_pipex **p);
int	restore_std(int *cpy_stdin, int *cpy_stdout);

/* LEXER */
int 	PIPE(LEXER *input, t_token **list);
int 	R_ARROW(LEXER *input);
int 	L_ARROW(LEXER *input);
int 	SP(LEXER *input);
int 	S_QUOTE(LEXER *input);
int 	D_QUOTE(LEXER *input);
int 	DOLLAR(LEXER *input);
int 	QUESTION_M(LEXER *input);
int 	LOW_ALPHA(LEXER *input);
int 	UP_ALPA(LEXER *input);
int 	DIGIT(LEXER *input);
int 	SLASH(LEXER *input);
int 	MINUS(LEXER *input);
int 	DOT(LEXER *input);
int 	UNDERSCORE(LEXER *input);
int 	BACKSLASH(LEXER *input);
int 	ESPER(LEXER *input);
int 	SEMI_COL(LEXER *input);
int 	PRINTABLE_SQUOTE(LEXER *input);
int 	PRINTABLE_DQUOTE(LEXER *input);
int 	HAT(LEXER *input);
int	EQUAL(LEXER *input);
int	PLUS(LEXER *input);
int	STAR(LEXER *input);
int	TAB_H(LEXER *input);
int	TAB_V(LEXER *input);
int	HASHTAG(LEXER *input);
int	DOTS(LEXER *input);

int	ows(LEXER *input);
int	squote(LEXER *input);
int	dquote(LEXER *input);
int	arg(LEXER *input);
int	redir(LEXER *input, t_token **list);
int	command(LEXER *input, t_token **list);
int	expr(LEXER *input, t_token **list);
int	start(LEXER *input);
int	parser_has_reach_end(LEXER *input);

extern int	g_signal;
#endif
