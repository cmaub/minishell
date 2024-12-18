/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:23:58 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 17:52:27 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exc_built(t_parser *current, t_pipex *p, t_cpy *cpy, t_mega *mini)
{
	if (ft_strncmp(current->command[0], "echo", 5) == 0)
		return (ft_echo(current->command));
	if (ft_strncmp(current->command[0], "pwd", 4) == 0)
		return (ft_pwd(current));
	if (ft_strncmp(current->command[0], "env", 4) == 0)
		return (ft_env(current, p->env_n));
	if (ft_strncmp(current->command[0], "exit", 5) == 0)
		return (ft_exit(p, current, cpy, mini));
	if (ft_strncmp(current->command[0], "cd", 3) == 0)
		return (ft_cd(current->command, p, current));
	if (ft_strncmp(current->command[0], "export", 7) == 0)
	{
		p->env_n = ft_export(current, p->env_n);
		return (TRUE);
	}
	if (ft_strncmp(current->command[0], "unset", 6) == 0)
	{
		p->env_n = ft_unset(current, p->env_n);
		return (TRUE);
	}
	return (FALSE);
}

int	is_builtin(t_parser *current)
{
	if (current->command != NULL)
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

int	handle_input_rediction_built(t_parser *nod, int *fd_in, int *d)
{
	if (nod->redir[nod->f] == REDIRECT_IN)
	{
		*fd_in = open(nod->file[nod->f], O_RDONLY | 0644);
		if (*fd_in == -1)
			return (perror(nod->file[nod->f]), FALSE);
		if (dup2(*fd_in, STDIN_FILENO) == -1)
			return (s_clse(fd_in), perror(nod->file[nod->f]), FALSE);
		s_clse(fd_in);
	}
	else if (nod->redir[nod->f] == HEREDOC)
	{
		s_clse(&nod->fd_heredoc[*d][1]);
		if (dup2(nod->fd_heredoc[*d][0], STDIN_FILENO) == -1)
		{
			s_clse(&nod->fd_heredoc[*d][0]);
			perror(nod->file[nod->f]);
			return (FALSE);
		}
		s_clse(&nod->fd_heredoc[*d][0]);
		d++;
	}
	return (TRUE);
}

int	redir_builtins(t_parser *nod, t_pipex *p, t_cpy *cpy, t_mega *mini)
{
	int	fd_in;
	int	d;

	fd_in = -1;
	nod->f = 0;
	p->flag = 1;
	d = 0;
	while (nod->file && nod->file[nod->f] != NULL)
	{
		if (!handle_input_rediction_built(nod, &fd_in, &d))
			return (FALSE);
		if (!handle_output_redirection(&nod, p, NULL))
			return (FALSE);
		nod->f++;
	}
	if (!exc_built(nod, p, cpy, mini))
		nod->exit_code = 1;
	return (TRUE);
}

int	handle_builtin(t_parser *node, t_pipex *p, t_mega *mini)
{
	t_cpy	cpy;

	cpy.cpy_stdin = -1;
	cpy.cpy_stdout = -1;
	if (cpy_std(&cpy.cpy_stdin, &cpy.cpy_stdout) == FALSE)
	{
		node->exit_code = 1;
		return (FALSE);
	}
	if (!redir_builtins(node, p, &cpy, mini))
	{
		restore_std(&cpy.cpy_stdin, &cpy.cpy_stdout);
		node->exit_code = 1;
		return (FALSE);
	}
	if (!restore_std(&cpy.cpy_stdin, &cpy.cpy_stdout))
	{
		node->exit_code = 1;
		return (FALSE);
	}
	if (p->exit == 1)
		(free(mini), exit(EXIT_SUCCESS));
	return (TRUE);
}
