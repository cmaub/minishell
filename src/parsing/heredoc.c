/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:38:55 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 14:41:04 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_c_signal_heredoc(int signum)
{
	g_signal = signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_done = 1;
	close(STDIN_FILENO);
}

int	loop_readline(char *delimiter, int *fd_heredoc)
{
	char	*input;

	g_signal = 0;
	signal(SIGINT, handle_c_signal_heredoc);
	while (g_signal == 0)
	{
		input = readline("heredoc> ");
		if (g_signal == SIGINT)
			return (free(input), s_clse(fd_heredoc), -1);
		if (!input)
		{
			ft_putendl_fd("warning: here-document delimited by end-of-file", 2);
			return (s_clse(fd_heredoc), 2);
		}
		if (ft_strncmp(input, delimiter, ft_strlen(delimiter)) == 0)
		{
			(s_clse(fd_heredoc), free(input));
			close(STDIN_FILENO); //tester /!/
			break ;
		}
		else
			ft_putendl_fd(input, *fd_heredoc);
		free(input);
	}
	return (0);
}

int	create_heredoc(t_parser *node, t_token *current, int *f, int *d)
{
	int	fd;
	int	result;

	// result = 0;
	fd = dup(STDIN_FILENO);
	if (fd == -1)
		return (perror("dup"), FALSE);
	signal(SIGINT, SIG_IGN);
	node->delimiter[*d] = ft_strdup(current->value);
	if (!node->delimiter[*d])
		return (s_clse(&fd), FALSE);
	result = loop_readline(node->delimiter[*d], &node->fd_heredoc[*d][1]);
	if (result == -1)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror("dup"), FALSE);
		return (s_clse(&fd), FALSE);
	}
	s_clse(&node->fd_heredoc[*d][1]);
	node->redir[*f] = current->type;
	signal(SIGINT, SIG_DFL);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror ("dup"), FALSE);
	if (result == 2)
	{
		dprintf(2, "COUCOU ctr-D\n");
		s_clse(&node->fd_heredoc[*d][0]);
	}
	return (s_clse(&fd), TRUE);
}

int	fill_nodes_with_heredoc(t_token **cur, t_parser **node, t_mega **mini)
{
	if (pipe((*node)->fd_heredoc[(*mini)->d]) == -1)
		return (perror("pipe"), FALSE);
	(*node)->file[(*mini)->f] = ft_strdup("heredoc");
	if (!create_heredoc((*node), (*cur), &(*mini)->f, &(*mini)->d))
	{
		close_heredoc(*node);
		// s_clse(&(*node)->fd_heredoc[(*mini)->d][0]);
		rst_nde_mini(*mini, node);
		free((*mini)->nodes);
		(*mini)->nodes = NULL;
		free((*node));
		(*node) = NULL;
		free_tokens(&(*mini)->tokens);
		if (g_signal == 2)
			(*mini)->exit_code = g_signal + 128;
		return (FALSE);
	}
	(*mini)->d++;
	(*mini)->f++;
	return (TRUE);
}
