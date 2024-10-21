/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:32:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/21 14:29:14 by anvander         ###   ########.fr       */
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

# define true 1
# define false 0

# define REDIRECT_IN 1 /* < */
# define REDIRECT_OUT 2 /* > */
# define APPEND_OUT 3 /* >> */
# define HEREDOC 4 /* << */
# define PIPE 5 /* | */
# define COMMAND 6
# define OPTION 7 /* - + taile de 3 */
# define ARGUMENT 8 
# define FILENAME 9 
# define TBD 10
# define DELIMITER 11

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
	struct s_token	*token;
	char	        	**envp;
	char			*heredoc;
	int		 	nb_cmd;
	int		       i;
	int		       prev_fd;
	pid_t	        	pid;
	pid_t	        	last_pid;
}	t_pipex;

char	*get_path_and_check(char **split_cmd, char **env);


void	add_new_token(t_token **list, t_token *new);
void	print_tokens_list(t_token *list);
void	ft_free_tab(char **tab);
void	ft_error(char *str);
void	check_open(int fd);
void	ft_init_struct(t_pipex *p, int ac, t_token *token, char **envp);
void	safe_close(int fd);
void	get_lines(t_token *current, int fd_heredoc);
void	handle_output_redirection(t_token *current, int pipefd[2], int fd_in);
void    ft_close_error(int *fd, int pipefd[2], char *str);

int	give_type_to_token(t_token *token);
int	list_size(t_token *list);
int	is_str(char *str);
int	simple_cmd(t_pipex *p, char *heredoc, int pipefd[2]);
int	no_envp(char **tab);
int	handle_input(t_token *token, char **envp, int ac);
int	handle_input_redirection(t_pipex *p, char *heredoc, int pipefd[2]);
int	execute(char *cmd, t_token *current, t_pipex *p);
int    ft_wait(pid_t last_pid);

t_token	*create_new_token(char *input);


#endif