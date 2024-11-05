/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:04:02 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/05 15:10:38 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	execute(PARSER *current, t_pipex *p)
{
	char	*path;

	if (ft_strchr(current->command[0], '/') || no_envp(p->envp))
	{
		if (access(current->command[0], F_OK | R_OK) == -1)
		{
			free(current);
			perror("access");
			return (-1);
		}
		execve(current->command[0], current->command, NULL);
	}
	else if (!ft_strchr(current->command[0], '/') && !no_envp(p->envp))
	{
		path = get_path_and_check(&current->command[0], p->envp);
		dprintf(2, "path = %s, split_cmd = %s, %s, %s, %s\n", path, current->command[0], current->command[1], current->command[2], current->command[3]);
		if (execve(path, current->command, p->envp) == -1)
		{
			free(current);
			free(path);
		}
	}
	perror("exec");
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
		// if ((*nodes)->redir_type_in[(*nodes)->i] == HEREDOC)
		// {
		// 	dprintf(2, "ligne = %d, (*nodes)->infile[(*nodes)->i] = %s\n", __LINE__, (*nodes)->infile[(*nodes)->i]);
		// 	unlink((*nodes)->infile[(*nodes)->i]);
		// }
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
		// if ((*nodes)->redir_type_out[(*nodes)->o] == REDIRECT_OUT)
		// 	fd_out = open((*nodes)->outfile[(*nodes)->o], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		// else if ((*nodes)->redir_type_out[(*nodes)->o] == APPEND_OUT)
		// 	fd_out = open((*nodes)->outfile[(*nodes)->o], O_WRONLY | O_CREAT | O_APPEND, 0644);
		// if (fd_out == -1)
		// {
		// 	safe_close(p->pipefd[1]);
		// 	safe_close(p->pipefd[0]);
		// 	ft_error("FIRST open out");
		// }
		// if (dup2(fd_out, STDOUT_FILENO) == -1)
		// {
		// 	safe_close(fd_out);
		// 	ft_close_error(&fd_in, p, "FIRST dup2 out");
		// }
		// safe_close(fd_out);
		(*nodes)->o++;
	}
	if (execute((*nodes), p) == -1)
		exit(EXIT_FAILURE);
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
		// if ((*nodes)->redir_type_in[(*nodes)->i] == HEREDOC)
		// 	unlink((*nodes)->infile[(*nodes)->i]);
		(*nodes)->i++;
	}
	if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
		ft_close_error(&fd_in, p, "INTER dup2 pipeout");
	safe_close(p->pipefd[1]);
	while ((*nodes)->outfile && (*nodes)->outfile[(*nodes)->o] != NULL)
	{
		handle_output_redirection(nodes, p, fd_in, fd_out);
		// if ((*nodes)->redir_type_out[(*nodes)->o] == REDIRECT_OUT)
		// 	fd_out = open((*nodes)->outfile[(*nodes)->o], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		// else if ((*nodes)->redir_type_out[(*nodes)->o] == APPEND_OUT)
		// 	fd_out = open((*nodes)->outfile[(*nodes)->o], O_WRONLY | O_CREAT | O_APPEND, 0644);
		// if (fd_out == -1)
		// {
		// 	ft_close_error(&fd_in, p, "FIRST dup2 out");
		// 	ft_error("FIRST open out");
		// }
		// if (dup2(fd_out, STDOUT_FILENO) == -1)
		// {
		// 	safe_close(fd_out);
		// 	ft_close_error(&fd_in, p, "FIRST dup2 out");
		// }
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
		// if ((*nodes)->redir_type_in[(*nodes)->i] == HEREDOC)
		// 	unlink((*nodes)->infile[(*nodes)->i]);
		(*nodes)->i++;
	}
	while ((*nodes)->outfile && (*nodes)->outfile[(*nodes)->o] != NULL)
	{
		handle_output_redirection(nodes, p, fd_in, fd_out);
		// if ((*nodes)->redir_type_out[(*nodes)->o] == REDIRECT_OUT)
		// 	fd_out = open((*nodes)->outfile[(*nodes)->o], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		// else if ((*nodes)->redir_type_out[(*nodes)->o] == APPEND_OUT)
		// 	fd_out = open((*nodes)->outfile[(*nodes)->o], O_WRONLY | O_CREAT | O_APPEND, 0644);
		// if (fd_out == -1)
		// {
		// 	ft_close_error(&fd_in, p, "FIRST dup2 out");
		// 	ft_error("FIRST open out");
		// }
		// if (dup2(fd_out, STDOUT_FILENO) == -1)
		// {
		// 	safe_close(fd_out);
		// 	ft_close_error(&fd_in, p, "FIRST dup2 out");
		// }
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
		{
			last_child(p, nodes);
		}
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

int	handle_input(PARSER **nodes, char **envp, int ac)
{
	t_pipex	*p;
	PARSER		*current;
	
	current = (*nodes); 
	p = malloc(sizeof(*p));
	if (!p)
		return (free (*nodes), free(current), 0);
	ft_init_struct(p, ac, envp, *nodes);
	while (p->i <= p->nb_cmd)
	{
		if(p->i < p->nb_cmd)
		{
			if (pipe(p->pipefd) == -1)
				ft_error("pipe");
		}
		p->pid = fork();
		if (p->i == p->nb_cmd)
			p->last_pid = p->pid;
		create_process(p, &current);
		p->i++;
		current = current->next;
		if (p->i == p->nb_cmd)
			break;	
	}
	return (ft_wait(p->last_pid, nodes));
}