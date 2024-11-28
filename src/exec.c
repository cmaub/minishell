/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:04:02 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/28 15:40:34 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipefds(t_pipex *p)
{
	if (p->pipefd[0] != -1)
	{
		close(p->pipefd[0]);
		p->pipefd[0] = -1;
	}
	if (p->pipefd[1] != -1)
	{
		close(p->pipefd[1]);
		p->pipefd[1] = -1;
	}
	if (p->prev_fd != -1)
	{
		close(p->prev_fd);
		p->prev_fd = -1;
	}
}

void	handle_output_redirection(PARSER **nodes, t_pipex *p, int fd_out)
{
	if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_OUT)
		fd_out = open((*nodes)->file[(*nodes)->f], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if ((*nodes)->redir_type[(*nodes)->f] == APPEND_OUT)
		fd_out = open((*nodes)->file[(*nodes)->f], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		safe_close(p->pipefd[1]);
		safe_close(p->pipefd[0]);
		ft_error((*nodes)->file[(*nodes)->f]);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		ft_close_error(&fd_out, p, (*nodes)->file[(*nodes)->f]);
	safe_close(fd_out);
}

int	exec_builtin(PARSER *current, t_pipex *p)
{
	if (ft_strncmp(current->command[0], "echo", 4) == 0)
		return (ft_echo(current->command));
	if (ft_strncmp(current->command[0], "pwd", 3) == 0)
		return (ft_pwd(current));
	if (ft_strncmp(current->command[0], "env", 3) == 0)
		return (ft_env(current, p->mini_env));
	if (ft_strncmp(current->command[0], "exit", 4) == 0)
		return (ft_exit(current->command, p, current));
	if (ft_strncmp(current->command[0], "cd", 2) == 0)
		return (ft_cd(current->command, p, current));
	if (ft_strncmp(current->command[0], "export", 6) == 0)
		return (ft_export(current, p->mini_env));
	if (ft_strncmp(current->command[0], "unset", 5) == 0)
	{
		p->mini_env = ft_unset(current, p->mini_env);
		return (TRUE);
	}
	return (FALSE);

}

int	is_builtin(PARSER *current)
{
	if (current->command)
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

void	is_command(PARSER *current)
{
	if (!current->command)
	{
		ft_putstr_fd(" ", 2);
		exit (0);
	}
	if ((current->command && current->command[0][0] == '\0'))
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(": command not found\n", 2);
		exit (127);
	}
}

int	execute(PARSER *current, t_pipex *p)
{
	char	*path;
	
	if (is_builtin(current) == 1)
	{
		exec_builtin(current, p);
		free(current);
		exit(EXIT_SUCCESS);
	}
	// if (!current->command/* || (current->command && current->command[0][0] == '\0')*/)
	// {
	// 	ft_putstr_fd(" ", 2);
	// 	// ft_putstr_fd(": command not found\n", 2);
	// 	return (-1);
	// }
	// if ((current->command && current->command[0][0] == '\0'))
	// {
	// 	ft_putstr_fd(" ", 2);
	// 	ft_putstr_fd(": command not found\n", 2);
	// 	return (-1);
	// }
	else if (ft_strchr(current->command[0], '/') || no_envp(p->mini_env))
	{
		if (access(current->command[0], F_OK) == 0)
		{
			if (access(current->command[0], R_OK) == -1)
			{
				perror(current->command[0]);
				exit(126);
			}
		}
		else
		{
			perror(current->command[0]);
			exit(127);
		}
		if (!no_envp(p->mini_env))
			execve(current->command[0], current->command, p->mini_env);
		else
			execve(current->command[0], current->command, NULL);
	}
	else if (!ft_strchr(current->command[0], '/') && !no_envp(p->mini_env))
	{			
		path = get_path_and_check(&current->command[0], p->mini_env);
		dprintf(2, "path = %s, split_cmd = %s, %s, %s, %s\n", path, current->command[0], current->command[1], current->command[2], current->command[3]);
		if (execve(path, current->command, p->mini_env) == -1)
			return (free(current), free(path), -1);
	}
	return (-1);
}

void	first_child(t_pipex *p, PARSER **nodes)
{
	int	fd_in;
	int	fd_out;
	
	fd_in = -1;
	fd_out = -1;
	(*nodes)->f = 0;
	safe_close(p->pipefd[0]);
	while ((*nodes)->file && (*nodes)->file[(*nodes)->f] != NULL)
	{
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
			fd_in = open((*nodes)->file[(*nodes)->f], O_RDONLY | 0644);
		if ((*nodes)->redir_type[(*nodes)->f] == HEREDOC)
			fd_in = open((*nodes)->file[(*nodes)->f], O_RDONLY | 0644);
		if (fd_in == -1 && ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN || (*nodes)->redir_type[(*nodes)->f] == HEREDOC))
		{	
			safe_close(p->pipefd[1]);
			ft_error((*nodes)->file[(*nodes)->f]);
		}
		if ((*nodes)->redir_type[(*nodes)->f] == HEREDOC || (*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
		{
			if (dup2(fd_in, STDIN_FILENO) == -1)
				ft_close_error(&fd_in, p, (*nodes)->file[(*nodes)->f]);
			safe_close(fd_in);
		}
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_OUT || (*nodes)->redir_type[(*nodes)->f] == APPEND_OUT)
			handle_output_redirection(nodes, p, fd_out);
		
		(*nodes)->f++;

	}
	if ((*nodes)->next)
	{
		if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
			ft_close_error(&fd_in, p, (*nodes)->file[(*nodes)->f]);
		safe_close(p->pipefd[1]);
	}
	is_command(*nodes);
	if (execute((*nodes), p) == -1)
		exit(127);
}

void	inter_child(t_pipex *p, PARSER **nodes)
{
	int	fd_in;
	int	fd_out;

	fd_in = -1;
	fd_out = -1;
	(*nodes)->f = 0;
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
	{
		close_pipefds(p);
		ft_error("pipe");
	}
	// close(p->pipefd[1]); // attention ne pas fermer
	// p->pipefd[1] = -1;
	close(p->pipefd[0]); 
	p->pipefd[0] = -1;
	close(p->prev_fd);
	p->prev_fd = -1;
	while ((*nodes)->file && (*nodes)->file[(*nodes)->f] != NULL)
	{
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
			fd_in = open((*nodes)->file[(*nodes)->f], O_RDONLY | 0644);
		if ((*nodes)->redir_type[(*nodes)->f] == HEREDOC)
			fd_in = open((*nodes)->file[(*nodes)->f], O_RDONLY | 0644);
		
		if (fd_in == -1 && ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN || (*nodes)->redir_type[(*nodes)->f] == HEREDOC))
		{
			safe_close(p->pipefd[1]);
			safe_close(p->pipefd[0]);
			ft_error((*nodes)->file[(*nodes)->f]);
		}
		if ((*nodes)->redir_type[(*nodes)->f] == HEREDOC || (*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
		{
			if (dup2(fd_in, STDIN_FILENO) == -1)
				ft_close_error(&fd_in, p, (*nodes)->file[(*nodes)->f]);
			safe_close(fd_in);
		}
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_OUT || (*nodes)->redir_type[(*nodes)->f] == APPEND_OUT)
			handle_output_redirection(nodes, p, fd_out);
		(*nodes)->f++;
	}
	if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
		ft_close_error(&fd_in, p, "pipe");
	safe_close(p->pipefd[1]);
	is_command(*nodes);
	if (execute((*nodes), p) == -1)
		exit(127);
}

void	last_child(t_pipex *p, PARSER **nodes)
{
	int	fd_in;
	int	fd_out;

	fd_in = -1;
	fd_out = -1;
	(*nodes)->f = 0;
	close(p->pipefd[1]);
	p->pipefd[1] = -1;
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
	{
		close_pipefds(p);
		ft_error("pipe");
	}
	close_pipefds(p);
	while ((*nodes)->file && (*nodes)->file[(*nodes)->f] != NULL)
	{
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
			fd_in = open((*nodes)->file[(*nodes)->f], O_RDONLY | 0644);
		if ((*nodes)->redir_type[(*nodes)->f] == HEREDOC)
			fd_in = open((*nodes)->file[(*nodes)->f], O_RDONLY | 0644);
		if (fd_in == -1 && ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN || (*nodes)->redir_type[(*nodes)->f] == HEREDOC))
		{
			safe_close(p->pipefd[1]);
			safe_close(p->pipefd[0]);
			ft_error((*nodes)->file[(*nodes)->f]);
		}
		if ((*nodes)->redir_type[(*nodes)->f] == HEREDOC || (*nodes)->redir_type[(*nodes)->f] == REDIRECT_IN)
		{
			if (dup2(fd_in, STDIN_FILENO) == -1)
				ft_close_error(&fd_in, p, (*nodes)->file[(*nodes)->f]);
			safe_close(fd_in);
		}
		if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_OUT || (*nodes)->redir_type[(*nodes)->f] == APPEND_OUT)
			handle_output_redirection(nodes, p, fd_out);
		(*nodes)->f++;
	}
	is_command(*nodes);
	if (execute((*nodes), p) == -1)
		exit(127);
}

void	parent_process(t_pipex *p)
{
	if (p->nb_cmd > 1)
	{
		if (p->prev_fd != - 1)
		{
			dprintf(2, "p->prev_fd = %d\n", p->prev_fd);
			close(p->prev_fd);
			p->prev_fd = -1;
		}
		if (p->i < p->nb_cmd -1)
		{
			safe_close(p->pipefd[1]);
			p->prev_fd = p->pipefd[0];
		}
		else
			close(p->pipefd[0]);
	}
}

void	create_process(t_pipex *p, PARSER **nodes)
{
	
	if (p->pid == -1)
		ft_error("fork");
	else if (p->pid == 0)
	{
		exit_status = 0;
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
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

int	cpy_std(int cpy_stdin, int cpy_stdout)
{
	cpy_stdin = dup(STDIN_FILENO);
	if (!cpy_stdin)
	{
		perror("cpy_stdin");
		return (FALSE);
	}
	cpy_stdout = dup(STDOUT_FILENO);
	if (!cpy_stdout)
	{
		perror("cpy_stdout");
		return (FALSE);
	}
	return (TRUE);
}

void	handle_simple_process(PARSER *current, t_pipex *p)
{
	int	fd_in;
	int	fd_out;
	int	cpy_stdin;
	int	cpy_stdout;
	
	cpy_stdout = -1;
	cpy_stdin = -1;
	fd_in = -1;
	fd_out = -1;
	current->f = 0;
	p->flag = 1;
	cpy_std(cpy_stdin, cpy_stdout); // a tester
	while (current->file && current->file[current->f] != NULL)
	{
		if (current->redir_type[current->f] == REDIRECT_IN )
			fd_in = open(current->file[current->f], O_RDONLY | 0644);
		if (current->redir_type[current->f] == HEREDOC)
			fd_in = open(current->file[current->f], O_RDONLY  | 0644);
		if (fd_in == -1 && (current->redir_type[current->f] == REDIRECT_IN || current->redir_type[current->f] == HEREDOC))
		{
			perror(current->file[current->f]);
			current->exit_code = 1;
			return ; //retablir stdin et out
		}
		if (current->redir_type[current->f] == HEREDOC || current->redir_type[current->f] == REDIRECT_IN)
		{
			
			if (dup2(fd_in, STDIN_FILENO) == -1)
			{
				safe_close(fd_in);
				perror(current->file[current->f]);
			}
			safe_close(fd_in);
		}
		if (current->redir_type[current->f] == REDIRECT_OUT || current->redir_type[current->f] == APPEND_OUT)
		{
			
			handle_output_redirection(&current, p, fd_out);
		}
		current->f++;
	}
	if (exec_builtin(current, p))
	{
		if (cpy_stdout != -1)
		{
			if (dup2(cpy_stdout, STDOUT_FILENO) == -1)
				perror("STDOUT");
			safe_close(cpy_stdout);
		}
		if (cpy_stdin != -1)
		{
			if (dup2(cpy_stdin, STDIN_FILENO) == -1)
				perror("STDIN");
			safe_close(cpy_stdin);
		}
		if (p->exit == 1)
			exit (EXIT_SUCCESS);
	}
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
				ft_error("pipe");
		}
		signal(SIGINT, SIG_IGN);
		p->pid = fork();
		if (p->i == p->nb_cmd - 1)
		{
			p->last_pid = p->pid;
		}
		create_process(p, &current);
		p->i++;
		current = current->next;
		if (p->i == p->nb_cmd)
			break;	
	}
	return (close_pipefds(p), ft_wait(p->last_pid, nodes));
}
