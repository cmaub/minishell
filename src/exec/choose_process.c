/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:27:55 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 17:53:09 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_child(t_pipex *p, t_parser **nodes, t_mega *mini)
{
	int	flag_output;

	(*nodes)->f = 0;
	flag_output = 0;
	s_clse(&p->pipefd[0]);
	while ((*nodes)->file && (*nodes)->file[(*nodes)->f] != NULL)
	{
		if (!handle_input_redirection(nodes, p, &p->d))
			free_exit(p, mini, EXIT_FAILURE);
		if (!handle_output_redirection(nodes, p, &flag_output))
			free_exit(p, mini, EXIT_FAILURE);
		(*nodes)->f++;
	}
	if (flag_output == 0 && (*nodes)->next)
	{
		if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
			(clse_n_x(NULL, p, "pipe"),
				free_exit(p, mini, EXIT_FAILURE));
	}
	s_clse(&p->pipefd[1]);
	if (!(*nodes)->command)
		free_exit(p, mini, EXIT_SUCCESS);
	close_all_heredoc(mini);
	if (!execute(nodes, p, mini))
		(msg_not_executable((*nodes)->command[0]), free_exit(p, mini, 126));
}

void	inter_child(t_pipex *p, t_parser **nodes, t_mega *mini)
{
	int	flag_output;

	(*nodes)->f = 0;
	flag_output = 0;
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
		(s_clse(&p->prev_fd), clse_n_x(NULL, p, "pipe"), free_exit(p, mini, 1));
	(s_clse(&p->pipefd[0]), s_clse(&p->prev_fd));
	while ((*nodes)->file && (*nodes)->file[(*nodes)->f] != NULL)
	{
		if (!handle_input_redirection(nodes, p, &p->d))
			free_exit(p, mini, EXIT_FAILURE);
		if (!handle_output_redirection(nodes, p, &flag_output))
			free_exit(p, mini, EXIT_FAILURE);
		(*nodes)->f++;
	}
	if (flag_output == 0 && (*nodes)->next)
	{
		if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
			(clse_n_x(NULL, p, "pipe"), free_exit(p, mini, 1));
	}
	(s_clse(&p->pipefd[1]), close_all_heredoc(mini));
	if (!(*nodes)->command)
		free_exit(p, mini, EXIT_SUCCESS);
	if (!execute(nodes, p, mini))
		(msg_not_executable((*nodes)->command[0]), free_exit(p, mini, 126));
}

void	last_child(t_pipex *p, t_parser **nodes, t_mega *mini)
{
	(*nodes)->f = 0;
	s_clse(&p->pipefd[1]);
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
	{
		clse_n_x(NULL, p, "pipe");
		free_exit(p, mini, EXIT_FAILURE);
	}
	close_pipefds(p);
	while ((*nodes)->file && (*nodes)->file[(*nodes)->f] != NULL)
	{
		if (!handle_input_redirection(nodes, p, &p->d))
			free_exit(p, mini, EXIT_FAILURE);
		if (!handle_output_redirection(nodes, p, NULL))
			free_exit(p, mini, EXIT_FAILURE);
		(*nodes)->f++;
	}
	if (!(*nodes)->command)
		free_exit(p, mini, EXIT_SUCCESS);
	close_all_heredoc(mini);
	if (!execute(nodes, p, mini))
	{
		msg_not_executable((*nodes)->command[0]);
		free_exit(p, mini, 126);
	}
}

void	parent_process(t_pipex *p)
{
	if (p->nb_cmd > 1)
	{
		if (p->prev_fd != -1)
		{
			s_clse(&p->prev_fd);
			p->prev_fd = -1;
		}
		if (p->i < p->nb_cmd -1)
		{
			s_clse(&p->pipefd[1]);
			p->prev_fd = p->pipefd[0];
		}
		else
			s_clse(&p->pipefd[0]);
	}
}

void	choose_process(t_pipex *p, t_parser **nodes, t_mega *mini)
{
	if (p->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (p->i == 0)
			first_child(p, nodes, mini);
		else if (p->i < p->nb_cmd - 1)
			inter_child(p, nodes, mini);
		else if (p->i == p->nb_cmd - 1)
			last_child(p, nodes, mini);
		exit (EXIT_SUCCESS);
	}
	else
	{
		parent_process(p);
		if ((*nodes)->fd_heredoc)
			s_clse_array((*nodes)->fd_heredoc, nodes);
	}
}
