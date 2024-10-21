/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:07:14 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/21 11:16:45 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	first_child(t_pipex *p, int pipefd[2], char **envp)
// {
// 	int	fd_in;

// 	fd_in = ft_open(av); //
// 	if (fd_in == -1)
// 	{
// 		safe_close(pipefd[1]);
// 		safe_close(pipefd[0]);
// 		ft_error("open");
// 	}
// 	if (dup2(fd_in, STDIN_FILENO) == -1)
// 		ft_close_error(&fd_in, pipefd, "dup2");
// 	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
// 		ft_close_error(&fd_in, pipefd, "dup2");
// 	safe_close(fd_in);
// 	safe_close(pipefd[1]);
// 	safe_close(pipefd[0]);
// 	if (ft_strncmp(av[1], "here_doc", 8) == 0) //
// 	{
// 		if (ft_execve(av[3], envp) == -1) //
// 			ft_error("First child heredoc execve");
// 	}
// 	else if (ft_execve(av[2], envp) == -1) //
// 		ft_error("First child execve");
// }

// void	inter_child(int *prev_fd, t_pipex *p, int pipefd[2], int i)
// {
// 	int	fd_in;

// 	fd_in = *prev_fd;
// 	if (dup2(fd_in, STDIN_FILENO) == -1)
// 		ft_close_error(&fd_in, pipefd, "dup2");
// 	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
// 		ft_close_error(&fd_in, pipefd, "dup2");
// 	safe_close(fd_in);
// 	safe_close(pipefd[1]);
// 	safe_close(pipefd[0]);
// 	if (ft_strncmp(p.av[1], "here_doc", 8) == 0)
// 	{
// 		if (ft_execve(p.av[i + 3], p.envp) == -1)
// 			ft_error("inter_child execve");
// 	}
// 	else if (ft_execve(p.av[i + 2], p.envp) == -1)
// 		ft_error("inter_child execve");
// }

// void	last_child(t_pipex p, int pipefd[2], char **envp, int ac)
// {
// 	int	fd_out;

// 	if (dup2(pipefd[0], STDIN_FILENO) == -1)
// 	{
// 		safe_close(pipefd[0]);
// 		safe_close(pipefd[1]);
// 		ft_error("dup2");
// 	}
// 	if (ft_strncmp(av[1], "here_doc", 8) == 0)
// 		fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else
// 		fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd_out == -1)
// 	{
// 		safe_close(pipefd[0]);
// 		safe_close(pipefd[1]);
// 		ft_error("open");
// 	}
// 	if (dup2(fd_out, STDOUT_FILENO) == -1)
// 		ft_close_error(&fd_out, pipefd, "dup2");
// 	safe_close(pipefd[0]);
// 	safe_close(fd_out);
// 	if (ft_execve(av[ac - 2], envp) == -1)
// 		ft_error("Last_child execve");
// }

// void	create_process(int pipefd[2], t_pipex *p, int *prev_fd, pid_t pid)
// {
// 	if (pid == -1)
// 		ft_error("fork");
// 	else if (pid == 0)
// 	{
// 		if (p->i == 0)
// 			first_child(p, pipefd, p->envp);
// 		else if (p->i < p->nb_cmd - 1)
// 			inter_child(prev_fd, p, pipefd, p->i);
// 		else if (p->i == p->nb_cmd - 1)
// 			last_child(p, pipefd, p->envp, p->ac);
// 	}
// 	else
// 	{
// 		if (*prev_fd != -1)
// 			safe_close(*prev_fd);
// 		if (p->i < p->nb_cmd - 1)
// 		{
// 			close(pipefd[1]);
// 			*prev_fd = pipefd[0];
// 		}
// 		else
// 			safe_close(pipefd[0]);
// 	}
// }

// int	pipex(t_pipex *p)
// {
// 	pid_t	pid;
// 	pid_t	last_pid;
// 	int		pipefd[2];
// 	int		exit_code;

// 	/* attention HEREDOC */
// 	while (p->i < p->nb_cmd)
// 	{
// 		if (p->i < p->nb_cmd - 1)
// 		{
// 			if (pipe(pipefd) == -1)
// 				ft_error("pipe");
// 		}
// 		pid = fork();
// 		if (p->i == p->nb_cmd - 1)
// 			last_pid = pid;
// 		create_process(pipefd, &p, &p->prev_fd, pid);
// 		p->i++;
// 	}
// 	return (ft_wait(last_pid));
// }

char	*ft_heredoc(t_token *token, char *heredoc)
{
	t_token	*current;
	int		fd_heredoc;

	current = token;
	fd_heredoc = -1;
	if (current->type == HEREDOC)
	{
		heredoc = "heredoc";
		fd_heredoc = open(heredoc, O_WRONLY | O_CREAT | O_APPEND, 0644);
		get_lines(current->next, fd_heredoc);
	}
	return (heredoc);
}

int	handle_input(t_token *token, char **envp, int ac)
{
	t_pipex	*p;
	char		*heredoc;	
	
	heredoc = NULL;
	p = malloc(sizeof(*p)); //penser a free
	ft_init_struct(p, ac, token, envp);
	heredoc = ft_heredoc(token, heredoc);
	// while (current)
	// {
	// 	if (current->type == PIPE)
	// 		return(pipex(p));
	// 	current = current->next;
	// }
	simple_cmd(p, heredoc);
	unlink(heredoc);
	return(0);
}
