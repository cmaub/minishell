/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:07:14 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/29 14:46:49 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_child(t_pipex *p)
{
	int		fd_in;
	t_token	*current;

	current = p->token;
	// fd_in = ft_open(av);
	safe_close(p->pipefd[0]);
	if (p->token->type == REDIRECT_IN || p->token->type == HEREDOC)
			fd_in = handle_input_redirection(p, p->heredoc);
	while (current->next && current->type != PIPEX)
		current = current->next;
	handle_output_redirection(current, p, fd_in);
	while (current->prev && current->type != COMMAND)
		current = current->prev;
	if (execute(current->value, current, p) == -1)
	{
		safe_close(p->pipefd[0]);
		safe_close(p->pipefd[1]);
		safe_close(fd_in);
		exit (EXIT_FAILURE);
	}
	while (current && current->type != PIPEX)
		current = current->next;
	p->token = current;
	dprintf(2, "p->token->value = %s\n", p->token->value);
}

void	inter_child(int *prev_fd, t_pipex *p, char *heredoc)
{
	int		fd_in;
	t_token	*current;
	

	current = p->token->next;
	if (p->token->type == HEREDOC)
		fd_in = handle_input_redirection(p, heredoc);
	else
	{
		fd_in = *prev_fd;
		if (dup2(fd_in, STDIN_FILENO) == -1)
			ft_close_error(&fd_in, p, "dup2");
	}
	handle_output_redirection(p->token, p, fd_in);
	while (current->prev && current->type != COMMAND)
		current = current->prev;
	safe_close(p->pipefd[0]);
	safe_close(p->pipefd[1]);
	safe_close(fd_in);
	if (execute(current->value, current, p) == -1)
		exit (EXIT_FAILURE);
	while (current && current->type != PIPEX)
		current = current->next;
	p->token = current;
	dprintf(2, "p->token->value = %s\n", p->token->value);
}

void	last_child(t_pipex *p, char *heredoc, int *prev_fd)
{
	int		fd_in;
	t_token	*current;
	(void)prev_fd;

	current = p->token->next;
	if (p->token->type == HEREDOC)
		fd_in = handle_input_redirection(p, heredoc);
	else
	{
		fd_in = *prev_fd;
		if (dup2(fd_in, STDIN_FILENO) == -1)
			ft_close_error(&fd_in, p, "dup2");
	}
	handle_output_redirection(current, p, fd_in);
	// safe_close(p->pipefd[0]);
	safe_close(p->pipefd[1]);
	safe_close(fd_in);
	while (current->prev && current->type != COMMAND)
		current = current->prev;
	if (execute(current->value, current, p) == -1)
		exit (EXIT_FAILURE);
}

void	create_process(t_pipex *p, int *prev_fd, pid_t pid, int count)
{
	if (pid == -1)
		ft_error("fork");
	else if (pid == 0)
	{
		if (p->i == 0)
			first_child(p);
		else if (p->i < count)
			inter_child(prev_fd, p, p->heredoc);
		else if (p->i == count)
			last_child(p, p->heredoc, prev_fd);
	}
	else
	{
		if (*prev_fd != -1)
			safe_close(*prev_fd);
		if (p->i <= count)
		{
			// close(pipefd[1]);
			*prev_fd = p->pipefd[0];
			// p->pipefd[0] = -1;
			// safe_close(pipefd[0]);
		
		}	
	}
}

char	*ft_heredoc(t_token **token)
{
	t_token	*current;
	int		fd_heredoc;
	char *heredoc;

	current = *token;
	fd_heredoc = -1;
	dprintf(2, "current->type = %d\n", current->type);
	while (current->prev && current->type != HEREDOC)
		current = current->prev;
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

int	pipex(t_pipex *p, int count)
{
	pid_t	pid;
	// pid_t	last_pid;

	while (p->i <= count)
	{
		if (p->i < count)
		{
			if (pipe(p->pipefd) == -1)
				ft_error("pipe");
		}
		pid = fork();
		if (p->i == count)
			p->last_pid = pid;
		create_process(p, &p->prev_fd, pid, count);
		p->i++;
	}
	p->i = 0;
	while (p->i <= count +1)
	{
		waitpid(pid, NULL, 0);
		p->i++;
	}
	return (0);
}

int	handle_input(t_token **token, char **envp, int ac)
{
	t_pipex	*p;
	t_token	*current;
	int		count;

	current = *token;
	count = 0;
	p = malloc(sizeof(*p));
	if (!p)
		return (free (token), free(current), 0);
	 // penser a free
	ft_init_struct(p, ac, token, envp);
	p->heredoc = ft_heredoc(token);
	dprintf(2, "p->heredoc = %s\n", p->heredoc);
	while (current)
	{
		if (current->type == PIPEX)
			count++;
		current = current->next;	
	}
	if (count > 0)
		pipex(p, count);
	else
		simple_cmd(p, p->heredoc);
	unlink(p->heredoc);
	return(0);
}
