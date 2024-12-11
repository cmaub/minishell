/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:04:02 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/11 18:20:25 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>


void	close_pipefds(t_pipex *p)
{
	if (p->pipefd[0] != -1)
	{
		safe_close(&p->pipefd[0]);
		// p->pipefd[0] = -1;
	}
	if (p->pipefd[1] != -1)
	{
		safe_close(&p->pipefd[1]);
		// p->pipefd[1] = -1;
	}
	if (p->prev_fd != -1)
	{
		safe_close(&p->prev_fd);
		// p->prev_fd = -1;
	}
}

int	handle_output_redirection(PARSER **nodes, t_pipex *p, int fd_out)
{
	if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_OUT)
		fd_out = open((*nodes)->file[(*nodes)->f], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if ((*nodes)->redir_type[(*nodes)->f] == APPEND_OUT)
		fd_out = open((*nodes)->file[(*nodes)->f], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		ft_close_error_no_exit(NULL, p, (*nodes)->file[(*nodes)->f]);
		return (FALSE);
	}
	dprintf(2, "fd_out = %d\n", fd_out);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		//dprintf(2, "dupe de stdout a fail\n");
		ft_close_error_no_exit(&fd_out, p, (*nodes)->file[(*nodes)->f]);
		return (FALSE);
	}
	safe_close(&fd_out);
	return (TRUE);
}

int	is_command(char *cmd)
{
	if (!cmd || cmd[0] == '\0' || ft_is_only_spaces(cmd))
	{
		return (FALSE);
	}
	return (TRUE);
	
}

int	exec_builtin(PARSER *current, t_pipex *p, int *cpy_stdin, int *cpy_stdout)
{
	if (ft_strncmp(current->command[0], "echo", 5) == 0)
		return (ft_echo(current->command));
	if (ft_strncmp(current->command[0], "pwd", 4) == 0)
		return (ft_pwd(current));
	if (ft_strncmp(current->command[0], "env", 4) == 0)
		return (ft_env(current, p->env_nodes));
	if (ft_strncmp(current->command[0], "exit", 5) == 0)
		return (ft_exit(current->command, p, current, cpy_stdin, cpy_stdout));
	if (ft_strncmp(current->command[0], "cd", 3) == 0)
		return (ft_cd(current->command, p, current));
	if (ft_strncmp(current->command[0], "export", 7) == 0)
	{
		p->env_nodes = ft_export(current, p->env_nodes);
		// dprintf(2, "env dans exec builtin\n");
		// print_t_env(p->env_nodes);
		return (TRUE);
	}
		// return (ft_export(current, p->env_nodes));
	if (ft_strncmp(current->command[0], "unset", 6) == 0)
	{
		p->env_nodes = ft_unset(current, p->env_nodes);
		return (TRUE);
	}
	return (FALSE);

}

int	is_builtin(PARSER *current)
{
	if (current->command != NULL)
	{
		if (ft_strncmp(current->command[0], "echo", 5) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "pwd", 4) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "env", 4) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "exit", 5) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "cd", 3) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "export", 7) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "unset", 6) == 0)
			return (TRUE);
	}
	return (FALSE);
}
// int	copy_list_in_str(char **str_env, t_env **env_nodes)
// {
// 	t_env	*temp;
// 	int		i;

// 	temp = *env_nodes;
// 	str_env = try_malloc(sizeof(char *) * (lstsize_t_env(env_nodes) + 1));
// 	if (!str_env)
// 		return (FALSE);
// 	i = 0;
// 	while (temp)
// 	{
// 		str_env[i] = ft_strdup(temp->var);
// 		// dprintf(2, "str_env[%d] = %s\n", i, str_env[i]);
// 		if (!str_env[i])
// 			return (FALSE); 
// 		i++;
// 		temp = temp->next;
// 	}
// 	str_env[i] = NULL;
// 	return (TRUE);
// }

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	// dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
	// dprintf(2, "avant boucle while tab[%d] = %s\n", i, tab[i]);
	while (tab[i] != NULL)
	{
		// dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
		dprintf(2, "tab[%d] = %s\n", i, tab[i]);
		i++;			
	}
	// dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
}

char	**copy_list_in_str(t_env **env_nodes)
{
	t_env	*temp;
	char	**str_env;
	int		i;

	if (!env_nodes || !*env_nodes)
		return (NULL);
	temp = *env_nodes;
	str_env = try_malloc(sizeof(char *) * (lstsize_t_env(env_nodes) + 1));
	if (!str_env)
		return (NULL);
	i = 0;
	while (temp)
	{
		str_env[i] = ft_strdup(temp->var);
		// dprintf(2, "str_env[%d] = %s\n", i, str_env[i]);
		if (!str_env[i])
			return (NULL); 
		i++;
		if (temp->next)
			temp = temp->next;
		else
			break ;
	}
	str_env[i] = NULL;
	// dprintf(2, "sous forme de tab\n");
	// print_tab(str_env);
	return (str_env);
}

int	execute(PARSER *current, t_pipex *p)
{
	char	*path;
	char	**tmp_cmd;
	char	**tmp_minienv;
	char	**str_env;
	
	tmp_cmd = NULL;
	tmp_minienv = NULL;
	str_env = NULL;
	if (is_builtin(current) == 1)
	{
		// dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
		exec_builtin(current, p, NULL, NULL); //verifier si on free bien les 2 structures
		reset_node(&current);
		free_pipex(&p);
		exit(EXIT_SUCCESS);
	}
	else
	{
		str_env = copy_list_in_str(p->env_nodes);
		if (current->command)
			tmp_cmd = copy_tab(current->command);
		if (!tmp_cmd)
			return (free_array(str_env), -1);
		free_t_env(p->env_nodes);
		free_pipex(&p);
		reset_node(&current);
		if (ft_strchr(tmp_cmd[0], '/') || no_envp(str_env))
		{
			if (access(tmp_cmd[0], F_OK) == 0)
			{
				if (access(tmp_cmd[0], R_OK) == -1)
				{
					perror(tmp_cmd[0]);
					free(tmp_cmd);
					free_array(str_env);
					// dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
					exit(126);
				}
			}
			else
			{
				perror(tmp_cmd[0]);
				free(tmp_cmd);
				free_array(str_env);
				// dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
				exit(127);
			}
			if (!no_envp(str_env))
			{
				// dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
				execve(tmp_cmd[0], tmp_cmd, str_env);
			}
			else
			{
				// dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
				execve(tmp_cmd[0], tmp_cmd, NULL);
			}
		}
		else if (!ft_strchr(tmp_cmd[0], '/') && !no_envp(str_env))
		{	
			// dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
			path = get_path_and_check(&tmp_cmd[0], str_env);
			// dprintf(2, "path = %s\n", path);
			// // dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
			// dprintf(2, "path = %s, split_cmd = %s, %s, %s, %s\n", path, tmp_cmd[0], tmp_cmd[1], tmp_cmd[2], tmp_cmd[3]);
			if (execve(path, tmp_cmd, str_env) == -1)
				return (free(tmp_cmd), free(path), free_array(str_env), -1);
		}
	}
	return (-1);
}

void	first_child(t_pipex *p, PARSER **nodes)
{
	int	fd_in;
	int	fd_out;
	int	d;
	int	flag_output;
	
	fd_in = -1;
	fd_out = -1;
	(*nodes)->f = 0;
	d = 0;
	flag_output = 0;
	safe_close(&p->pipefd[0]);
	while ((*nodes)->file && (*nodes)->file[(*nodes)->f] != NULL)
	{
		// //dprintf(2, "(*nodes)->redir_type[%d] = %d\n", (*nodes)->f, (*nodes)->redir_type[(*nodes)->f]);
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
			fd_in = open((*nodes)->file[(*nodes)->f], O_RDONLY | 0644);
		if (fd_in == -1 && (*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
		{
			close_error_and_free(NULL, p, nodes, (*nodes)->file[(*nodes)->f], 1);
		}
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
		{
			dprintf(2, "fd_in = %d\n", fd_in);
			if (dup2(fd_in, STDIN_FILENO) == -1)
			{
				// reset_node(nodes);
				close_error_and_free(&fd_in, p, nodes, (*nodes)->file[(*nodes)->f], 1);
			}
			safe_close(&fd_in);
		}
		if ((*nodes)->redir_type[(*nodes)->f] == HEREDOC)
		{
			safe_close(&(*nodes)->fd_heredoc[d][1]);
			if (dup2((*nodes)->fd_heredoc[d][0], STDIN_FILENO) == -1)
			{
				// //dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
				// reset_node(nodes); //deja free avec le close_error
				close_error_and_free(&(*nodes)->fd_heredoc[d][0], p, nodes, (*nodes)->file[(*nodes)->f], 1);//rajouter fd heredoc
			}
			safe_close(&(*nodes)->fd_heredoc[d][0]);
			d++;
		}
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_OUT || (*nodes)->redir_type[(*nodes)->f] == APPEND_OUT)
		{
			if (!handle_output_redirection(nodes, p, fd_out))
			{
				free_pipex(&p);
				reset_node(nodes);
				exit(1) ;
			}
			flag_output = 1;
		}
		(*nodes)->f++;

	}
	if (flag_output == 0 && (*nodes)->next)
	{
		if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
		{
			//dprintf(2, "c'est ici le pb\n");
			close_error_and_free(&fd_in, p, nodes, (*nodes)->file[(*nodes)->f], 1);
		}
		safe_close(&p->pipefd[1]);
	}
	if (execute((*nodes), p) == -1)
	{
		ft_putstr_fd((*nodes)->command[0], 2);
		ft_putendl_fd(": not executable", 2);
		exit(126);
	}
}

void	inter_child(t_pipex *p, PARSER **nodes)
{
	int	fd_in;
	int	fd_out;
	int	d;
	int	flag_output;

	fd_in = -1;
	fd_out = -1;
	(*nodes)->f = 0;
	d = 0;
	flag_output = 0;
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
		close_error_and_free(NULL, p, nodes, "pipe", 1);
	safe_close(&p->pipefd[0]); 
	safe_close(&p->prev_fd);
	while ((*nodes)->file && (*nodes)->file[(*nodes)->f] != NULL)
	{
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
			fd_in = open((*nodes)->file[(*nodes)->f], O_RDONLY | 0644);
		if (fd_in == -1 && (*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
			close_error_and_free(NULL, p, nodes, (*nodes)->file[(*nodes)->f], 1);
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
		{
			if (dup2(fd_in, STDIN_FILENO) == -1)
				close_error_and_free(&fd_in, p, nodes,(*nodes)->file[(*nodes)->f], 1);
			safe_close(&fd_in);
		}
		if ((*nodes)->redir_type[(*nodes)->f] == HEREDOC)
		{
			safe_close(&(*nodes)->fd_heredoc[d][1]);
			if (dup2((*nodes)->fd_heredoc[d][0], STDIN_FILENO) == -1)
			{
				// reset_node(nodes);
				close_error_and_free(&(*nodes)->fd_heredoc[d][0], p, nodes, (*nodes)->file[(*nodes)->f], 1);//rajouter fd heredoc
			}
			safe_close(&(*nodes)->fd_heredoc[d][0]);
			d++;
		}
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_OUT || (*nodes)->redir_type[(*nodes)->f] == APPEND_OUT)
		{
			if (!handle_output_redirection(nodes, p, fd_out))
			{
				free_pipex(&p);
				reset_node(nodes);
				exit(1);
			}
			flag_output = 1;
		}
		(*nodes)->f++;
	}
	if (flag_output == 0 && (*nodes)->next)
	{
		if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
			close_error_and_free(&fd_in, p, nodes, "pipe", 1);
	}
	safe_close(&p->pipefd[1]);
	if (execute((*nodes), p) == -1)
	{
		ft_putstr_fd((*nodes)->command[0], 2);
		ft_putendl_fd(": not executable", 2);
		exit(126);
	}
}

void	last_child(t_pipex *p, PARSER **nodes)
{
	int	fd_in;
	int	fd_out;
	int 	d;

	fd_in = -1;
	fd_out = -1;
	(*nodes)->f = 0;
	d = 0;
	safe_close(&p->pipefd[1]);
	// p->pipefd[1] = -1;
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
		close_error_and_free(NULL, p, nodes, "pipe", 1);
	close_pipefds(p);
	while ((*nodes)->file && (*nodes)->file[(*nodes)->f] != NULL)
	{
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
			fd_in = open((*nodes)->file[(*nodes)->f], O_RDONLY | 0644);
		if (fd_in == -1 && (*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
			close_error_and_free(NULL, p, nodes, (*nodes)->file[(*nodes)->f], 1);
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
		{
			if (dup2(fd_in, STDIN_FILENO) == -1)
				close_error_and_free(&fd_in, p, nodes, (*nodes)->file[(*nodes)->f], 1);

			safe_close(&fd_in);
		}
		if ((*nodes)->redir_type[(*nodes)->f] == HEREDOC)
		{
			safe_close(&(*nodes)->fd_heredoc[d][1]);
			if (dup2((*nodes)->fd_heredoc[d][0], STDIN_FILENO) == -1)
			{
				// //dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
				// reset_node(nodes);
				close_error_and_free(&(*nodes)->fd_heredoc[d][0], p, nodes, (*nodes)->file[(*nodes)->f], 1);
			}
			safe_close(&(*nodes)->fd_heredoc[d][0]);
			d++;
		}
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_OUT || (*nodes)->redir_type[(*nodes)->f] == APPEND_OUT)
		{
			if (!handle_output_redirection(nodes, p, fd_out))
			{
				free_pipex(&p);
				reset_node(nodes);
				exit(1);
			}
		}
		(*nodes)->f++;
	}
	if (execute((*nodes), p) == -1)
	{
		ft_putstr_fd((*nodes)->command[0], 2);
		ft_putendl_fd(": not executable", 2);
		exit(126);
	}
}

void	parent_process(t_pipex *p)
{
	if (p->nb_cmd > 1)
	{
		if (p->prev_fd != - 1)
		{
			safe_close(&p->prev_fd);
			p->prev_fd = -1;
		}
		if (p->i < p->nb_cmd -1)
		{
			safe_close(&p->pipefd[1]);
			p->prev_fd = p->pipefd[0];
		}
		else
			safe_close(&p->pipefd[0]);
	}
}

void	create_process(t_pipex *p, PARSER **nodes)
{
	
	if (p->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (p->i == 0)
			first_child(p, nodes);
		else if (p->i < p->nb_cmd - 1)
			inter_child(p, nodes);
		else if (p->i == p->nb_cmd - 1)
			last_child(p, nodes);
		exit (EXIT_SUCCESS);
	}
	else 
		parent_process(p);
}

int	cpy_std(int *cpy_stdin, int *cpy_stdout)
{
	*cpy_stdin = dup(STDIN_FILENO);
	if (!*cpy_stdin)
	{
		perror("cpy_stdin");
		return (FALSE);
	}
	// safe_close(cpy_stdin);
	*cpy_stdout = dup(STDOUT_FILENO);
	if (!*cpy_stdout)
	{
		perror("cpy_stdout");
		return (FALSE);
	}
	// safe_close(cpy_stdout);
	return (TRUE);
}

int	restore_std(int *cpy_stdin, int *cpy_stdout)
{
	if (*cpy_stdout != -1)
	{
		if (dup2(*cpy_stdout, STDOUT_FILENO) == -1)
		{
			perror("STDOUT");
			// safe_close(*cpy_stdin);
			// return (FALSE);
		}
		safe_close(cpy_stdout);
		*cpy_stdout = -1;
	}
	if (*cpy_stdin != -1)
	{
		if (dup2(*cpy_stdin, STDIN_FILENO) == -1)
		{
			perror("STDIN");
			return (FALSE);
		}
		safe_close(cpy_stdin);
		*cpy_stdin = -1;
	}
	if (*cpy_stdout != -1)
		return (FALSE);
	// //dprintf(2, "cpy_stdin = %d et cpy_stdout = %d\n", *cpy_stdin, *cpy_stdout);
	// //dprintf(2, "stdin = %d et stdout = %d\n", STDIN_FILENO, STDOUT_FILENO);
	return (TRUE);
}

void	handle_simple_process(PARSER *current, t_pipex *p)
{
	int	fd_in;
	int	fd_out;
	int	cpy_stdin;
	int	cpy_stdout;
	int	d;
	
	cpy_stdout = -1;
	cpy_stdin = -1;
	fd_in = -1;
	fd_out = -1;
	current->f = 0;
	p->flag = 1;
	d = 0;
	if (cpy_std(&cpy_stdin, &cpy_stdout) == FALSE)
	{	
		current->exit_code = 1;
		return ;
	}
	while (current->file && current->file[current->f] != NULL)
	{
		if (current->redir_type[current->f] == REDIRECT_IN )
			fd_in = open(current->file[current->f], O_RDONLY | 0644);
		if (fd_in == -1 && current->redir_type[current->f] == REDIRECT_IN)
		{
			perror(current->file[current->f]);
			current->exit_code = 1;
			restore_std(&cpy_stdin, &cpy_stdout);
			return ;
		}
		if (current->redir_type[current->f] == REDIRECT_IN)
		{
			if (dup2(fd_in, STDIN_FILENO) == -1)
			{
				restore_std(&cpy_stdin, &cpy_stdout);
				safe_close(&fd_in);
				current->exit_code = 1;
				perror(current->file[current->f]);
				return ;
			}
			safe_close(&fd_in);
		}
		if (current->redir_type[current->f] == HEREDOC)
		{
			safe_close(&current->fd_heredoc[d][1]);
			if (dup2(current->fd_heredoc[d][0], STDIN_FILENO) == -1)
			{
				restore_std(&cpy_stdin, &cpy_stdout);
				safe_close(&current->fd_heredoc[d][0]);
				current->exit_code = 1;
				perror(current->file[current->f]);
				return ;
			}
			safe_close(&current->fd_heredoc[d][0]);
			d++;
		}
		if (current->redir_type[current->f] == REDIRECT_OUT || current->redir_type[current->f] == APPEND_OUT)
		{
			if (!handle_output_redirection(&current, p, fd_out))
			{
				current->exit_code = 1;
				restore_std(&cpy_stdin, &cpy_stdout);
				return ; //verifier si acces et return
			}
		}
		current->f++;
	}
	
	if (!exec_builtin(current, p, &cpy_stdin, &cpy_stdout))
		current->exit_code = 1;
	if (restore_std(&cpy_stdin, &cpy_stdout) == FALSE)
	{
		current->exit_code = 1;
		return ;
	}
	if (p->exit == 1)
		exit(EXIT_SUCCESS);
}

int	handle_input(PARSER **nodes, t_pipex *p)
{
	PARSER		*current;
	
	current = (*nodes);
	if (current && current->next == NULL && is_builtin(current))
		return (handle_simple_process(current, p), 0);
	while (p->i < p->nb_cmd)
	{
		if(p->nb_cmd > 1 && p->i < p->nb_cmd - 1)
		{
			if (pipe(p->pipefd) == -1)
			{
				perror("pipe");
				(*nodes)->exit_code = 1;
				return (FALSE);
			}
		}
		signal(SIGINT, SIG_IGN);
		p->pid = fork();
		if (p->pid == -1)
		{
			perror("fork");
			close_pipefds(p);
			(*nodes)->exit_code = 1;
			return (FALSE);
		}
		if (p->i == p->nb_cmd - 1)
			p->last_pid = p->pid;
		create_process(p, &current);
		p->i++;
		current = current->next;
		if (p->i == p->nb_cmd)
			break;	
	}
	
	return (close_pipefds(p), ft_wait(p->last_pid, nodes));
}
