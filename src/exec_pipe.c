/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:07:14 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/31 17:58:12 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_child(t_pipex *p, PARSER *current, PARSER *nodes)
{
	int		fd_in;

	current->cmd = 0;
	// safe_close(p->pipefd[0]);
	if (current->redir_type_in == REDIRECT_IN || current->redir_type_in == HEREDOC)
			fd_in = handle_input_redirection(p, current, p->heredoc);
	handle_output_redirection(current, nodes, p, fd_in);
	
	if (execute(current, p) == -1)
	{
		safe_close(p->pipefd[0]);
		safe_close(p->pipefd[1]);
		safe_close(fd_in);
		exit (EXIT_FAILURE);
	}
	current = current->next;
}

void	inter_child(int *prev_fd, t_pipex *p, char *heredoc, PARSER *current, PARSER *nodes)
{
	int	fd_in;
	(void)nodes;
	(void)prev_fd;
	
	if (current->redir_type_in == HEREDOC || current->redir_type_in == REDIRECT_IN)
		fd_in = handle_input_redirection(p, current, heredoc);
	else
	{
		// fd_in = *prev_fd;
		// fd_in = p->pipefd[0];
		dprintf(2, "pipefd[0] = %d, ligne %d\n", p->pipefd[0], __LINE__);
		if (dup2(p->pipefd[0], STDIN_FILENO) == -1)
		{
			dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
			ft_close_error(&p->pipefd[0], p, "dup2");
		}
	}
	if (current->outfile)
		handle_output_redirection(current, nodes, p, fd_in);
	else 
	{
		if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
			ft_close_error(&fd_in, p, "dup2");
	}
	safe_close(fd_in); //
	
	safe_close(p->pipefd[1]);
	if (execute(current, p) == -1)
		exit (EXIT_FAILURE);
}

void	last_child(t_pipex *p, char *heredoc, int *prev_fd, PARSER *current, PARSER *nodes)
{
	int		fd_in;
	(void)prev_fd;

	if (current->redir_type_in == HEREDOC || current->redir_type_in == REDIRECT_IN)
		fd_in = handle_input_redirection(p, current, heredoc);
	else
	{
		// fd_in = *prev_fd;
		fd_in = p->pipefd[0];
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
			ft_close_error(&fd_in, p, "dup2");
		}
	}
	if (current->outfile)
		handle_output_redirection(current, nodes, p, fd_in);
	safe_close(p->pipefd[1]);
	if (execute(current, p) == -1)
		exit (EXIT_FAILURE);
}

void	create_process(t_pipex *p, int *prev_fd, pid_t pid, PARSER *current, PARSER *nodes)
{
	if (pid == -1)
		ft_error("fork");
	else if (pid == 0)
	{
		dprintf(2, "prev_fd = %d et pipefd[0] = %d, ligne %d\n", *prev_fd, p->pipefd[0], __LINE__);
		safe_close(p->pipefd[0]);
		if (p->i == 0)
		{
			first_child(p, current, nodes);
		}
		else if (p->i < nodes->index - 1)
		{
			p->pipefd[0] = *prev_fd;
			dprintf(2, "prev_fd = %d et pipefd[0] = %d, ligne %d\n", *prev_fd, p->pipefd[0], __LINE__);
			// *prev_fd = p->pipefd[0];
			inter_child(prev_fd, p, p->heredoc, current, nodes);
		}
		else if (p->i == nodes->index - 1)
		{
			p->pipefd[0] = *prev_fd;
			dprintf(2, "prev_fd = %d et pipefd[0] = %d, ligne %d\n", *prev_fd, p->pipefd[0], __LINE__);			
			// *prev_fd = p->pipefd[0];
			last_child(p, p->heredoc, prev_fd, current, nodes);
		}
		exit(0);//
	}
	else
	{
		if (*prev_fd != -1)
		{
			safe_close(*prev_fd);
		}
		if (p->i > 0)
		{
			// safe_close(p->pipefd[1]);
			dprintf(2, "prev_fd = %d et pipefd[0] = %d, ligne %d\n", *prev_fd, p->pipefd[0], __LINE__);
			*prev_fd = dup(p->pipefd[0]);
			dprintf(2, "prev_fd = %d et pipefd[0] = %d, ligne %d\n", *prev_fd, p->pipefd[0], __LINE__);
		}
	}
	// 	// else
	// 	// 	safe_close(p->pipefd[0]);
	// 	// if (p->i < nodes->index -1)
	// 	// 	safe_close(p->pipefd[1]);
		
	// }
}

// char	*ft_heredoc(t_token **token)
// {
// 	t_token	*current;
// 	int		fd_heredoc;
// 	char *heredoc;

// 	current = *token;
// 	fd_heredoc = -1;
// 	dprintf(2, "current->type = %d\n", current->type);
// 	while (current->prev && current->type != HEREDOC)
// 		current = current->prev;
// 	if (current->type == HEREDOC)
// 	{
// 		heredoc = "heredoc";
// 		fd_heredoc = open(heredoc, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		get_lines(current->next, fd_heredoc);
// 	}
// 	else
// 		return (NULL);
// 	return (heredoc);
// }

int	pipex(t_pipex *p, PARSER *nodes, PARSER *current)
{
	// pid_t	pids[nodes->index];
	pid_t	pid;

	while (p->i < nodes->index)
	{
		if (p->i < nodes->index - 1)
		{
			if (pipe(p->pipefd) == -1)
				ft_error("pipe");
		}
		pid = fork();
		if (pid == -1)
			ft_error("fork");
		// pids[p->i] = pid;
		if (p->i == nodes->index - 1)
			p->last_pid = pid;
		create_process(p, &p->prev_fd, pid, current, nodes);
		p->i++;
		if (p->i == nodes->index - 1)
			break ;
		current = current->next;
	}
	// safe_close(p->prev_fd);
	safe_close(p->pipefd[1]);
	// safe_close(p->pipefd[0]);
	ft_wait(p->last_pid);
	return (0);
}

int	handle_input(PARSER *nodes, char **envp, int ac)
{
	t_pipex	*p;
	PARSER		*current;

	current = nodes->next; //il semblerait qu'il y ait un premier noeud vide donc on passe direct a next
	p = malloc(sizeof(*p));
	if (!p)
		return (free (nodes), free(current), 0);
	 // penser a free
	ft_init_struct(p, ac, envp);
	// p->heredoc = ft_heredoc(token);
	if (nodes->index > 1)
		pipex(p, nodes, current);
	else
		simple_cmd(p, p->heredoc, current, nodes);
	unlink(p->heredoc);
	dprintf(2, "fin de handle input (%s, %d)\n", __FILE__, __LINE__);
	return(0);
}
