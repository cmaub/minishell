/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:32:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/06 18:43:37 by anvander         ###   ########.fr       */
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

// < file1 cat > file2 >> file3 

// infile[0] = "file1"
// outfile[0] = "file2"
// outfile[1] = "file2"
// command[0] = "cat"

// redir_type_in[0] = REDIRECT_IN;
// redir_type_out[0] = REDIRECT_OUT;
// redir_type_out[1] = APPEND_OUT;

// i = 0;

// while (infile[j])
// {
// 	j++;
// }
// fonction_infile(infile[j], redir_type_in[j]);

typedef struct PARSER
{
	char	**infile; // ne dup que le dernier
	char	**outfile; // ne dup que le dernier
	char	**command; // n'exec que la premiere
	char	**delimiter;
	int		*redir_type_in;
	int		*redir_type_out;
	int		*fd_heredoc;
	int		cmd;
	int		i;
	int		o;
	int	nb_infile;
	int	nb_outfile;
	int	nb_command;
	int	nb_heredoc;
	struct PARSER	*next;
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
	int		       i;
	int		       prev_fd;
	int			pipefd[2];
	pid_t	        	pid;
	pid_t	        	last_pid;
}	t_pipex;


int create_and_add_token(LEXER *input, int start, int end, t_token **list, int type);


/* MINISHELL */
char	*get_path_and_check(char **split_cmd, char **env);
char	**copy_env(char **envp);

void	add_new_token(t_token **list, t_token *new);
void	print_tokens_list(t_token **list);
void	ft_free_tab(char **tab);
void	ft_error(char *str);
void	check_open(int fd);
void	ft_init_struct(t_pipex *p, int ac, char **envp, PARSER *nodes);
void	safe_close(int fd);
void	get_lines(PARSER *nodes, int i, int d);
void	handle_output_redirection(PARSER **nodes, t_pipex *p, int fd_in, int fd_out);
void   ft_close_error(int *fd, t_pipex *p, char *str);
void	replace_prev_token(t_token **list, t_token *new);

int	give_type_to_token(t_token *token);
// int	list_size(t_token *list);
int		ft_size_list(PARSER **nodes);
int	is_str(char *str);
int	simple_cmd(t_pipex *p, char *heredoc, PARSER *current, PARSER *nodes);
int	no_envp(char **tab);
int	handle_input(PARSER **nodes, char **envp, int ac);
int	ft_here_doc(PARSER *nodes);
int	handle_input_redirection(t_pipex *p, PARSER *current, char *heredoc);
int	execute(PARSER *current, t_pipex *p);
int    ft_wait(pid_t last_pid, PARSER **nodes);

t_token	*create_new_token(LEXER *input, int start, int end, int type);
void	print_nodes_list(PARSER **nodes);
void	add_new_node(PARSER **nodes, PARSER *new_node);

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

int ows(LEXER *input);
int squote(LEXER *input);
int dquote(LEXER *input);
int arg(LEXER *input, t_token **list);
int redir(LEXER *input, t_token **list);
int command(LEXER *input, t_token **list);
int expr(LEXER *input, t_token **list);
int start(LEXER *input);

#endif