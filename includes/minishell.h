/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander <anvander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:32:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 15:43:08 by anvander         ###   ########.fr       */
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
# define ARG 8
# define FILENAME 9
# define TBD 10
# define DELIMITER 11

# define FALSE 0
# define TRUE 1

typedef struct s_cpy
{
	int cpy_stdin;
	int cpy_stdout;
} t_cpy;

typedef struct s_parser
{
	char **file;
	char	**command;
	char	**delimiter;
	int		*redir;
	int		cmd;
	int		f;
	int	nb_command;
	int	nb_file;
	int	nb_heredoc;
	struct s_parser	*next;
	int			exit_code;
	int	**fd_heredoc;
} t_parser ;

typedef struct s_lexer 
{
    char    *data;
    size_t  len;
    size_t  head;
} t_lexer;

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
	t_env			**env_n;
}	t_pipex;

typedef struct s_sig
{
	int	sigint;
	int	sigquit;
	pid_t	pid;
	
}t_sig;

typedef struct s_mega
{
	t_parser *nodes;
	t_parser *begin;
	t_token *tokens;
	t_lexer	*L_input;
	t_env	**chained_env;
	t_pipex *p;
	int	exit_code;
	char	*str;
	int	f;
	int	cmd;
	int	d;
	int	idx;
} t_mega;


void	try_find_cmd_file(char **tmp_cmd, char **str_env);

// ENV
int	lstsize_t_env(t_env **lst);
void	ft_lstadd_env_back(t_env **lst, t_env *new);
void	add_new_var(t_env **mini_env, t_env *new_var);
void	print_t_env(t_env **mini_env);
int	count_env_var(char **list);
int	env_var_exists(t_env **env_n, char *var);
char	*return_var_from_env(char *str, t_env **chained_env);
t_env	**copy_t_env(t_env **env);
char	**find_path_line(char **env);

/* MINISHELL */
char	*get_path_and_check(char **split_cmd, char **env);
char	**copy_tab(char **envp);
int	ft_error_int(char *str, t_parser *node);
void	ft_error_exit(char *str, int exit_c);
void	clse_n_x(int *fd, t_pipex *p, char *str);
void	check_open(int fd);
void	ft_init_struct(t_pipex *p, t_env **chained_env, t_parser *nodes);
void	s_clse(int *fd);
// int		handle_output_redirection(t_parser **nodes, t_pipex *p, int fd_out);
int		handle_output_redirection(t_parser **nodes, t_pipex *p, int *flag_output);
void	handle_c_signal(int signum);
void	handle_c_signal_wait(int signum);
// void	handle_c_signal_child(int signum);
// void	check_signal_handler();

/* TOKENS */
int	fill_list_of_tokens(t_mega *mini, char *str);
int	no_envp(char **tab);
// int	handle_input(t_parser **nodes, t_pipex *p);
int	handle_input(t_parser **nodes, t_pipex *p, t_mega *mini);
int	ft_here_doc(t_parser *nodes);
// int	execute(t_parser *current, t_pipex *p);
// int	execute(t_parser *current, t_pipex *p, t_mega *mini);
int	execute(t_parser **current, t_pipex *p, t_mega *mini);
int    ft_wait(pid_t last_pid, t_parser **nodes);
int	add_new(t_token **list, t_token *new);
void	print_tokens_list(t_token **list);
t_token	*create_new(t_lexer *input, int start, int end, int type);
// int create_and_add_token(t_lexer *input, int start, int end, t_token **list, int type);

/* NODES */
int	create_nodes(t_mega *mini);
void	print_nodes_list(t_parser **nodes);
// void	add_new_node(t_parser **nodes, t_parser *new_node);
void	add_new_node(t_mega *mini, t_parser **nodes, t_parser *new);
int	ft_size_list(t_parser **nodes);
int	is_command(char *cmd);

/* BUILTINS */
int	ft_echo(char **cmd);
int	ft_pwd(t_parser *current);
int	ft_env(t_parser *current, t_env **env);
int	ft_exit(t_pipex *p, t_parser *node, t_cpy *cpy, t_mega *mini);
int	ft_cd(char **cmd, t_pipex *p, t_parser *node);
int	create_new_var(t_env **node, char *str);
int	env_var_exists(t_env **env_n, char *var);
void	print_sorted_env(t_env **env_n);
t_env	**ft_unset(t_parser *current, t_env **env_n);
t_env	**ft_export(t_parser *current, t_env **env_n);

/* ALLOC AND FREE */
void	ft_free_tab(char **tab);
char	**copy_tab_free(char **envp);
void	free_new_node(t_parser *new_node);
void	rst_nde(t_parser **node);
void	rst_nde_mini(t_mega *mini, t_parser **node);
void	free_tokens(t_token **tokens);
void	check_and_free_new_node(t_parser *new_node);
void	free_env(t_env **mini_env);
void	free_array_int(int **array, t_parser *current);
void	reset_one_node(t_parser **node);
// void   close_error_and_free(int *fd, t_pipex *p, t_parser **nodes, char *str, int exit_c);
void	close_error_and_free(int *fd, t_mega *mini, char *str, int exit_c);
void	free_array(char **array);
void	free_pipex(t_pipex **p);
int	restore_std(int *cpy_stdin, int *cpy_stdout);
void	free_exit(t_pipex *p, t_mega *mini, int exit_c);
void	close_heredoc(t_parser *current);

/* t_lexer */
int 	ft_pipe(t_lexer *input, t_token **list);
int 	ft_r_arrow(t_lexer *input);
int 	ft_l_arrow(t_lexer *input);
int 	ft_space(t_lexer *input);
int 	ft_squote(t_lexer *input);
int 	ft_dquote(t_lexer *input);
int 	ft_dollar(t_lexer *input);
int 	ft_question(t_lexer *input);
int 	ft_l_alpha(t_lexer *input);
int 	ft_u_alpha(t_lexer *input);
int 	ft_digit(t_lexer *input);
int 	ft_slash(t_lexer *input);
int 	ft_min(t_lexer *input);
int 	ft_dot(t_lexer *input);
int 	ft_undescore(t_lexer *input);
int 	print_ft_squote(t_lexer *input);
int 	print_ft_dquote(t_lexer *input);
int 	ft_hat(t_lexer *input);
int	ft_equal(t_lexer *input);
int	ft_plus(t_lexer *input);
int	ft_star(t_lexer *input);
int	ft_tab_h(t_lexer *input);
int	ft_tab_v(t_lexer *input);
int	ft_dots(t_lexer *input);

int	eat(t_lexer *input, char c);
int	eat_range(t_lexer *input, int start, int end);
int	ows(t_lexer *input);
int	squote(t_lexer *input);
int	dquote(t_lexer *input);
int	arg(t_lexer *input);
int	redir(t_lexer *input, t_token **list);
int	command(t_lexer *input, t_token **list);
int	expr(t_lexer *input, t_token **list);
int	start(t_lexer *input);
int	parser_has_reach_end(t_lexer *input);

extern int	g_signal;
#endif
