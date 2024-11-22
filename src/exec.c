/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:04:02 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/22 17:32:09 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_output_redirection(PARSER **nodes, t_pipex *p, int fd_out)
{
	if ((*nodes)->redir_type[(*nodes)->f] == REDIRECT_OUT)
		fd_out = open((*nodes)->file[(*nodes)->f], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if ((*nodes)->redir_type[(*nodes)->f] == APPEND_OUT)
		fd_out = open((*nodes)->file[(*nodes)->f], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		dprintf(2, " p->pipefd[0] = %d, p->pipefd[1] = %d\n", p->pipefd[0], p->pipefd[1]);
		safe_close(p->pipefd[1]);
		safe_close(p->pipefd[0]);
		ft_error((*nodes)->file[(*nodes)->f]);
		// ft_close_error(&fd_in, p, "FIRST open out");
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		ft_close_error(&fd_out, p, (*nodes)->file[(*nodes)->f]);
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
		if (ft_strncmp(current->command[0], "echo", 4) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "pwd", 3) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "env", 3) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "exit", 4) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "cd", 2) == 0) //TRUE ?
			return (TRUE);
		else if (ft_strncmp(current->command[0], "export", 6) == 0)
			return (TRUE);
		else if (ft_strncmp(current->command[0], "unset", 5) == 0)
			return (TRUE);
	}
	return (FALSE);
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
	else if (ft_strchr(current->command[0], '/') || no_envp(p->mini_env))
	{
		if (access(current->command[0], F_OK) == 0)
		{
			if (access(current->command[0], R_OK) == -1)
			{
				perror("access"); // modification message derreur ?
				exit(126);
			}
		}
		else
		{
			perror("access"); // modification message derreur ?
			exit(127);
		}
		execve(current->command[0], current->command, NULL);
	}
	else if (!ft_strchr(current->command[0], '/') && !no_envp(p->mini_env))
	{		
		path = get_path_and_check(&current->command[0], p->mini_env);
		dprintf(2, "path = %s, split_cmd = %s, %s, %s, %s\n", path, current->command[0], current->command[1], current->command[2], current->command[3]);
		if (execve(path, current->command, p->mini_env) == -1)
		{
			ft_putstr_fd("execve fail\n", 2);
			free(current);
			free(path);
			return (-1); // modifier exit code
		}
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
		close(p->pipefd[0]);
		close(p->prev_fd);
		close(p->pipefd[1]);
		ft_error("pipe");
	}
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
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
	{
		close(p->pipefd[0]);
		close(p->prev_fd);
		close(p->pipefd[1]);
		ft_error("pipe");
	}
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
	if (execute((*nodes), p) == -1)
		exit(127);
}

void handle_c_signal_child(int signum)
{
    (void)signum;
	exit_status = 0;
	exit(0);
}

void	create_process(t_pipex *p, PARSER **nodes)
{
	if (p->pid == -1)
		ft_error("fork");
	else if (p->pid == 0)
	{
		exit_status = 0;
		signal(SIGINT, handle_c_signal_child);
		signal(SIGQUIT, SIG_IGN);
		if (p->i == 0)
			first_child(p, nodes);
		else if (p->i < p->nb_cmd - 1)
			inter_child(p, nodes);
		else if (p->i == p->nb_cmd - 1)
			last_child(p, nodes);
	}
	else
	{
		if (p->prev_fd != - 1)
			close (p->prev_fd);
		if (p->i < p->nb_cmd -1)
		{
			close (p->pipefd[1]);
			p->prev_fd = p->pipefd[0];
		}
		else
			close(p->pipefd[0]);
	}
}

void	handle_simple_process(PARSER *current, t_pipex *p)
{
	int	fd_in;
	int	fd_out;
	int	cpy_stdin;
	int	cpy_stdout;
	
	cpy_stdin = dup(STDIN_FILENO);
	cpy_stdout = dup(STDOUT_FILENO);
	fd_in = -1;
	fd_out = -1;
	current->f = 0;
	p->flag = 1;
	while (current->file && current->file[current->f] != NULL)
	{
		if (current->redir_type[current->f] == REDIRECT_IN )
			fd_in = open(current->file[current->f], O_RDONLY | 0644);
		if (current->redir_type[current->f] == HEREDOC)
			fd_in = open(current->file[current->f], O_RDONLY  | 0644);
		if (fd_in == -1 && (current->redir_type[current->f] == REDIRECT_IN || current->redir_type[current->f] == HEREDOC))
			ft_error(current->file[current->f]);
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
				handle_output_redirection(&current, p, fd_out);
		current->f++;
	}
	if (exec_builtin(current, p))
	{
		free(current);
		if (dup2(cpy_stdout, STDOUT_FILENO) == -1)
			perror("STDOUT");
		if (dup2(cpy_stdin, STDIN_FILENO) == -1)
			perror("STDIN");
		if (p->exit == 1)
			exit (EXIT_SUCCESS);
	}
}

int	handle_input(PARSER **nodes, t_pipex *p)
{
	PARSER		*current;
	
	current = (*nodes);
	if (current->next == NULL && is_builtin(current))
		return (handle_simple_process(current, p), 0);
	while (p->i < p->nb_cmd)
	{
		if(p->nb_cmd > 1 && p->i < p->nb_cmd)
		{
			if (pipe(p->pipefd) == -1)
				ft_error("pipe");
		}
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
	return (ft_wait(p->last_pid, nodes));
}
