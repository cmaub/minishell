/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:07:14 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/21 15:57:20 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_child(t_pipex *p, int pipefd[2])
{
	int		fd_in;
	t_token	*current;

	current = p->token;
	// fd_in = ft_open(av);
	if (p->token->type == REDIRECT_IN || p->token->type == HEREDOC)
			fd_in = handle_input_redirection(p, p->heredoc, pipefd);

	while (current->next && current->type != PIPE)
		current = current->next;
	handle_output_redirection(current, pipefd, fd_in);

	while (current->prev && current->type != COMMAND)
		current = current->prev;
	if (execute(current->value, current, p) == -1)
	{
		safe_close(pipefd[0]);
		safe_close(pipefd[1]);
		safe_close(fd_in);
		exit (EXIT_FAILURE);
	}
	while (current && current->type != PIPE)
		current = current->next;
	p->token = current;
	dprintf(2, "p->token->value = %s\n", p->token->value);
}

void	inter_child(int *prev_fd, t_pipex *p, int pipefd[2], char *heredoc)
{
	int		fd_in;
	t_token	*current;
	

	current = p->token->next;
	if (p->token->type == HEREDOC)
		fd_in = handle_input_redirection(p, heredoc, pipefd);
	else
	{
		fd_in = *prev_fd;
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			dprintf(2, "Hey INTER \n");
			ft_close_error(&fd_in, pipefd, "dup2");
		}
	}
	handle_output_redirection(p->token, pipefd, fd_in);
	while (current->prev && current->type != COMMAND)
		current = current->prev;
	if (execute(current->value, current, p) == -1)
	{
		safe_close(pipefd[0]);
		safe_close(pipefd[1]);
		safe_close(fd_in);
		exit (EXIT_FAILURE);
	}
	while (current && current->type != PIPE)
		current = current->next;
	p->token = current;
	dprintf(2, "p->token->value = %s\n", p->token->value);
}

void	last_child(t_pipex *p, int pipefd[2], char *heredoc, int *prev_fd)
{
	int		fd_in;
	t_token	*current;
	(void)prev_fd;

	current = p->token->next;
	if (p->token->type == HEREDOC)
	{
		fd_in = handle_input_redirection(p, heredoc, pipefd);
	
	}
	else
	{
		// fd_in = *prev_fd;
		// if (dup2(fd_in, STDIN_FILENO) == -1)
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			dprintf(2, "Hey LAST \n");
			ft_close_error(&fd_in, pipefd, "dup2");	
		}
	}

	handle_output_redirection(current, pipefd, fd_in);

	while (current->prev && current->type != COMMAND)
		current = current->prev;

	if (execute(current->value, current, p) == -1)
	{
		safe_close(pipefd[0]);
		safe_close(pipefd[1]);
		safe_close(fd_in);
		exit (EXIT_FAILURE);
	}
}

void	create_process(int pipefd[2], t_pipex *p, int *prev_fd, pid_t pid, int count)
{
	if (pid == -1)
		ft_error("fork");
	else if (pid == 0)
	{
		if (p->i == 0)
			first_child(p, pipefd);
		else if (p->i < count)
			inter_child(prev_fd, p, pipefd, p->heredoc);
		else if (p->i == count)
			last_child(p, pipefd, p->heredoc, prev_fd);
	}
	else
	{
		if (*prev_fd != -1)
		{
			safe_close(*prev_fd);
		
		}
		if (p->i <= count)
		{
			// safe_close(pipefd[1]);
			*prev_fd = pipefd[0];
			pipefd[0] = -1;
			// safe_close(pipefd[0]);
		
		}	
	}
}

char	*ft_heredoc(t_token *token)
{
	t_token	*current;
	int		fd_heredoc;
	char *heredoc;

	current = token;
	fd_heredoc = -1;
	if (current->type == HEREDOC)
	{
		heredoc = "heredoc";
		fd_heredoc = open(heredoc, O_WRONLY | O_CREAT | O_APPEND, 0644);
		get_lines(current->next, fd_heredoc);
	}
	else
		return (NULL);
	return (heredoc);
}
int	pipex(t_pipex *p, int count, int pipefd[2])
{
	pid_t	pid;
	pid_t	last_pid;

	while (p->i <= count)
	{
		pid = fork();
		if (p->i == count)
			last_pid = pid;
		create_process(pipefd, p, &p->prev_fd, pid, count);
		p->i++;
	}
	return (ft_wait(last_pid));
}

int	handle_input(t_token *token, char **envp, int ac)
{
	t_pipex	*p;
	t_token	*current;
	int		count;
	int		pipefd[2];
	
	current = token;
	pipefd[0] = -1;
	pipefd[1] = -1;
	count = 0;
	p = malloc(sizeof(*p));
	if (!p)
		return (free (token), free(current), 0);
	 //penser a free
	ft_init_struct(p, ac, token, envp);
	p->heredoc = ft_heredoc(token);
	while (current)
	{
		if (current->type == PIPE)
		{
			count++;
			if (pipe(pipefd) == -1)
				ft_error("pipe");
			dprintf(2, "nombre de pipe %d\n", count);
		}
		current = current->next;	
	}
	pipex(p, count, pipefd);
	simple_cmd(p, p->heredoc, pipefd);
	unlink(p->heredoc);
	return(0);
}
