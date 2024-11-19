/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:04:02 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/19 19:11:35 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// extern int	exit_code;

void	handle_output_redirection(PARSER **nodes, t_pipex *p, int fd_in, int fd_out)
{
	if ((*nodes)->redir_type_out[(*nodes)->o] == REDIRECT_OUT)
		fd_out = open((*nodes)->outfile[(*nodes)->o], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if ((*nodes)->redir_type_out[(*nodes)->o] == APPEND_OUT)
		fd_out = open((*nodes)->outfile[(*nodes)->o], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		ft_close_error(&fd_in, p, "FIRST dup2 out");
		ft_error("FIRST open out");
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		safe_close(fd_out);
		ft_close_error(&fd_in, p, "FIRST dup2 out");
	}
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
	if (ft_strncmp(current->command[0], "echo", 4) == 0)
		return (TRUE);
	if (ft_strncmp(current->command[0], "pwd", 3) == 0)
		return (TRUE);
	if (ft_strncmp(current->command[0], "env", 3) == 0)
		return (TRUE);
	if (ft_strncmp(current->command[0], "exit", 4) == 0)
		return (TRUE);
	if (ft_strncmp(current->command[0], "cd", 2) == 0) //TRUE ?
		return (TRUE);
	if (ft_strncmp(current->command[0], "export", 6) == 0)
		return (TRUE);
	if (ft_strncmp(current->command[0], "unset", 5) == 0)
		return (TRUE);
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
		if (access(current->command[0], F_OK | R_OK) == -1)
			return (free(current), perror("access"), -1);
		execve(current->command[0], current->command, NULL);
	}
	else if (!ft_strchr(current->command[0], '/') && !no_envp(p->mini_env))
	{		
		path = get_path_and_check(&current->command[0], p->mini_env);
		if(!path)
			return (-1);
		dprintf(2, "path = %s, split_cmd = %s, %s, %s, %s\n", path, current->command[0], current->command[1], current->command[2], current->command[3]);
		path[0] = 'D';
		if (execve(path, current->command, p->mini_env) == -1)
		{
			// tester execve sur bash pour gerer l'exit code
			// trouver la liste des codes derreur de bash
			// current->exit_code = 1;
			// perror("execve");
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
	(*nodes)->i = 0;
	(*nodes)->o = 0;
	safe_close(p->pipefd[0]);
	while ((*nodes)->infile && (*nodes)->infile[(*nodes)->i] != NULL)
	{
		if ((*nodes)->redir_type_in[(*nodes)->i] == REDIRECT_IN)
			fd_in = open((*nodes)->infile[(*nodes)->i], O_RDONLY | 0644);
		if ((*nodes)->redir_type_in[(*nodes)->i] == HEREDOC)
			fd_in = open((*nodes)->infile[(*nodes)->i], O_RDONLY | 0644);
		if (fd_in == -1)
		{	
			safe_close(p->pipefd[1]);
			safe_close(p->pipefd[0]);
			ft_error("FIRST open in");
		}
		if (dup2(fd_in, STDIN_FILENO) == -1)
			ft_close_error(&fd_in, p, "FIRST dup2 in");
		safe_close(fd_in);
		(*nodes)->i++;
	}
	if ((*nodes)->next)
	{
		if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
			ft_close_error(&fd_in, p, "FIRST dup2 out");
		safe_close(p->pipefd[1]);
	}
	while ((*nodes)->outfile && (*nodes)->outfile[(*nodes)->o] != NULL)
	{
		handle_output_redirection(nodes, p, fd_in, fd_out);
		(*nodes)->o++;
	}	
	// if builtin ?
	if (execute((*nodes), p) == -1)
	{
		exit(EXIT_FAILURE);
	}
}

void	inter_child(t_pipex *p, PARSER **nodes)
{
	int	fd_in;
	int	fd_out;

	fd_in = -1;
	fd_out = -1;
	(*nodes)->i = 0;
	(*nodes)->o = 0;
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
	{
		close(p->pipefd[0]);
		close(p->prev_fd);
		close(p->pipefd[1]);
		ft_error("LAST dup2 pipein");
	}
	while ((*nodes)->infile && (*nodes)->infile[(*nodes)->i] != NULL)
	{
		if ((*nodes)->redir_type_in[(*nodes)->i] == REDIRECT_IN)
			fd_in = open((*nodes)->infile[(*nodes)->i], O_RDONLY | 0644);
		if ((*nodes)->redir_type_in[(*nodes)->i] == HEREDOC)
			fd_in = open((*nodes)->infile[(*nodes)->i], O_RDONLY  | 0644); // revenir sur e nom du fichier temp ?
		if (fd_in == -1)
		{
			safe_close(p->pipefd[1]);
			safe_close(p->pipefd[0]);
			ft_error("FIRST open in");
		}
		if (dup2(fd_in, STDIN_FILENO) == -1)
			ft_close_error(&fd_in, p, "FIRST dup2 in");
		safe_close(fd_in);
		(*nodes)->i++;
	}
	if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
		ft_close_error(&fd_in, p, "INTER dup2 pipeout");
	safe_close(p->pipefd[1]);
	while ((*nodes)->outfile && (*nodes)->outfile[(*nodes)->o] != NULL)
	{
		handle_output_redirection(nodes, p, fd_in, fd_out);
		(*nodes)->o++;
	}
	if (execute((*nodes), p) == -1)
		exit(EXIT_FAILURE);
}

void	last_child(t_pipex *p, PARSER **nodes)
{
	int	fd_in;
	int	fd_out;

	fd_in = -1;
	fd_out = -1;
	(*nodes)->i = 0;
	(*nodes)->o = 0;
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
	{
		close(p->pipefd[0]);
		close(p->prev_fd);
		close(p->pipefd[1]);
		ft_error("LAST dup2 pipein");
	}
	while ((*nodes)->infile && (*nodes)->infile[(*nodes)->i] != NULL)
	{
		if ((*nodes)->redir_type_in[(*nodes)->i] == REDIRECT_IN )
			fd_in = open((*nodes)->infile[(*nodes)->i], O_RDONLY | 0644);
		if ((*nodes)->redir_type_in[(*nodes)->i] == HEREDOC)
			fd_in = open((*nodes)->infile[(*nodes)->i], O_RDONLY  | 0644); // revenir sur e nom du fichier temp ?
		if (fd_in == -1)
		{
			safe_close(p->pipefd[1]);
			safe_close(p->pipefd[0]);
			ft_error("FIRST open in");
		}
		if (dup2(fd_in, STDIN_FILENO) == -1)
			ft_close_error(&fd_in, p, "FIRST dup2 in");
		safe_close(fd_in);
		(*nodes)->i++;
	}
	while ((*nodes)->outfile && (*nodes)->outfile[(*nodes)->o] != NULL)
	{
		handle_output_redirection(nodes, p, fd_in, fd_out);
		(*nodes)->o++;
	}
	if (execute((*nodes), p) == -1)
		exit(EXIT_FAILURE);
}

void	create_process(t_pipex *p, PARSER **nodes)
{
	if (p->pid == -1)
		ft_error("fork");
	else if (p->pid == 0)
	{
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

	fd_in = -1;
	fd_out = -1;
	current->i = 0;
	current->o = 0;
	p->flag = 1;
	while (current->infile && current->infile[current->i] != NULL)
	{
		if (current->redir_type_in[current->i] == REDIRECT_IN )
			fd_in = open(current->infile[current->i], O_RDONLY | 0644);
		if (current->redir_type_in[current->i] == HEREDOC)
			fd_in = open(current->infile[current->i], O_RDONLY  | 0644); // revenir sur e nom du fichier temp ?
		if (fd_in == -1)
		{
			ft_error("FIRST open in");
		}
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			safe_close(fd_in);
			perror("dup2");
		}
			
		safe_close(fd_in);
		current->i++;
	}
	while (current->outfile && current->outfile[current->o] != NULL)
	{
		handle_output_redirection(&current, p, fd_in, fd_out);
		current->o++;
	}
	if (exec_builtin(current, p))
	{
		free(current);
		if (p->exit == 1)
			exit (EXIT_FAILURE);
	}
}

int	handle_input(PARSER **nodes, t_pipex *p)
{
	PARSER		*current;
	
	current = (*nodes); 
	if (current->next == NULL && is_builtin(current))
	{
		handle_simple_process(current, p);
		return (0);
	}
	while (p->i < p->nb_cmd)
	{
		if(p->i < p->nb_cmd)
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
