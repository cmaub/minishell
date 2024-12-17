/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:04:02 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/17 18:26:27 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

void	close_pipefds(t_pipex *p)
{
	if (p->pipefd[0] != -1)
		safe_close(&p->pipefd[0]);
	if (p->pipefd[1] != -1)
		safe_close(&p->pipefd[1]);
	if (p->prev_fd != -1)
		safe_close(&p->prev_fd);
}

int	is_command(char *cmd)
{
	if (!cmd || cmd[0] == '\0' || ft_is_only_spaces(cmd))
	{
		return (FALSE);
	}
	return (TRUE);
}

int	exec_builtin(PARSER *current, t_pipex *p, t_cpy *cpy, t_mega_struct *mini)
{
	if (ft_strncmp(current->command[0], "echo", 5) == 0)
		return (ft_echo(current->command));
	if (ft_strncmp(current->command[0], "pwd", 4) == 0)
		return (ft_pwd(current));
	if (ft_strncmp(current->command[0], "env", 4) == 0)
		return (ft_env(current, p->env_nodes));
	if (ft_strncmp(current->command[0], "exit", 5) == 0)
		return (ft_exit(p, current, cpy, mini));
	if (ft_strncmp(current->command[0], "cd", 3) == 0)
		return (ft_cd(current->command, p, current));
	if (ft_strncmp(current->command[0], "export", 7) == 0)
	{
		p->env_nodes = ft_export(current, p->env_nodes);
		return (TRUE);
	}
	if (ft_strncmp(current->command[0], "unset", 6) == 0)
	{
		p->env_nodes = ft_unset(current, p->env_nodes);
		return (TRUE);
	}
	return (FALSE);
}

int	is_builtin(PARSER *current)
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

char	**copy_list_in_str(t_env **env_nodes)
{
	t_env	*temp;
	char	**str_env;
	int		i;

	if (!env_nodes || !*env_nodes)
		return (NULL);
	temp = *env_nodes;
	str_env = try_malloc(sizeof(char *) * (lstsize_t_env(env_nodes) + 1));
	if (!str_env)
		return (NULL);
	i = 0;
	while (temp)
	{
		str_env[i] = ft_strdup(temp->var);
		if (!str_env[i])
			return (NULL);
		i++;
		if (temp->next)
			temp = temp->next;
		else
			break ;
	}
	str_env[i] = NULL;
	return (str_env);
}

int	find_path(char **env)
{
	int	i;

	i = 0 ;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

void	free_exit_tab_str(char **env, char **cmd, char *other_cmd, int code)
{
	if (cmd[0])
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": not executable", 2);
		free_array(cmd);
	}
	else
		ft_putendl_fd("execve failed", 2);
	if (env)
		free_array(env);
	if (other_cmd)
		free(other_cmd);
	exit(code);
}

void	try_find_cmd_file(char **tmp_cmd, char **str_env)
{
	char	*dir_cmd;
	char	*curr_dir;

	curr_dir = "./";
	dir_cmd = ft_strjoin(curr_dir, tmp_cmd[0]);
	if (!dir_cmd)
		free_exit_tab_str(tmp_cmd, str_env, NULL, 1);
	if (access(dir_cmd, F_OK) == 0)
	{
		if (access(dir_cmd, R_OK) == -1)
			free_exit_tab_str(str_env, tmp_cmd, dir_cmd, 126);
		if (find_path(str_env))
		{
			if (execve(dir_cmd, tmp_cmd, str_env) == -1)
				free_exit_tab_str(str_env, tmp_cmd, dir_cmd, 126);
		}
		else
			if (execve(dir_cmd, tmp_cmd, NULL) == -1)
				free_exit_tab_str(str_env, tmp_cmd, dir_cmd, 126);
	}
	else
		free_exit_tab_str(str_env, tmp_cmd, dir_cmd, 127);
}

void	exec_no_env_or_path(char **tmp_cmd, char **str_env, t_mega_struct *mini)
{
	if (access(tmp_cmd[0], F_OK) == 0)
	{
		if (access(tmp_cmd[0], R_OK) == -1)
			(free(mini), free_exit_tab_str(str_env, tmp_cmd, NULL, 126));
	}
	else
		try_find_cmd_file(tmp_cmd, str_env);
	if (find_path(str_env))
	{
		if (execve(tmp_cmd[0], tmp_cmd, str_env) == -1)
			(free(mini), free_exit_tab_str(str_env, tmp_cmd, NULL, 126));
	}
	else
	{
		if (execve(tmp_cmd[0], tmp_cmd, NULL) == -1)
			(free(mini), free_exit_tab_str(str_env, tmp_cmd, NULL, 126));
		free(mini);
		exit(126);
	}
}

void	exec_without_pb(char **tmp_cmd, char **str_env, t_mega_struct *mini)
{
	char	*path;

	path = get_path_and_check(&tmp_cmd[0], str_env);
	if (execve(path, tmp_cmd, str_env) == -1)
	{
		ft_putstr_fd(tmp_cmd[0], 2);
		ft_putendl_fd(": not executable", 2);
		free_array(tmp_cmd);
		free(path);
		free_array(str_env);
		free(mini);
	}
	exit(126);
}

int	execute(PARSER **current, t_pipex *p, t_mega_struct *mini)
{
	char	**tmp_cmd;
	char	**tmp_minienv;
	char	**str_env;

	tmp_cmd = NULL;
	tmp_minienv = NULL;
	str_env = NULL;
	if (is_builtin(*current) == 1)
		(exec_builtin(*current, p, NULL, mini), reset_node(&mini->begin), free_t_env(p->env_nodes),
			free_pipex(&p), free(mini), exit(EXIT_SUCCESS));
	else
	{
		str_env = copy_list_in_str(p->env_nodes);
		if (!str_env)
			return (FALSE);
		tmp_cmd = copy_tab((*current)->command);
		if (!tmp_cmd)
			return (free_array(str_env), FALSE);
		(free_t_env(p->env_nodes), free_pipex(&p), reset_node(&mini->begin), free(mini));
		if (ft_strchr(tmp_cmd[0], '/') || !find_path(str_env))
			exec_no_env_or_path(tmp_cmd, str_env, mini);
		else if (!ft_strchr(tmp_cmd[0], '/') && find_path(str_env))
			exec_without_pb(tmp_cmd, str_env, mini);
	}
	return (FALSE);
}

int	handle_input_redirection(PARSER **n, t_pipex *p, int *d)
{
	int	fd_in;

	fd_in = -1;
	if ((*n)->redir[(*n)->f] == REDIRECT_IN)
			fd_in = open((*n)->file[(*n)->f], O_RDONLY | 0644);
	if (fd_in == -1 && (*n)->redir[(*n)->f] == REDIRECT_IN)
	{
		clse_n_qit(NULL, p, (*n)->file[(*n)->f]);
		return (FALSE);
	}
	if ((*n)->redir[(*n)->f] == REDIRECT_IN)
	{

		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			clse_n_qit(&fd_in, p, (*n)->file[(*n)->f]);
			return (FALSE);
		}
		safe_close(&fd_in);
	}
	if ((*n)->redir[(*n)->f] == HEREDOC)
	{
		safe_close(&(*n)->fd_heredoc[*d][1]);
		if (dup2((*n)->fd_heredoc[*d][0], STDIN_FILENO) == -1)
		{
			clse_n_qit(&(*n)->fd_heredoc[*d][0], p, (*n)->file[(*n)->f]);
			return (FALSE);
		}
		safe_close(&(*n)->fd_heredoc[*d][0]);
		*d += 1;
	}
	return (TRUE);
}

int	handle_output_redirection(PARSER **n, t_pipex *p, int *flag_output)
{
	int	fd_out;

	fd_out = -1;
	if ((*n)->redir[(*n)->f] != REDIRECT_OUT
		&& (*n)->redir[(*n)->f] != APPEND_OUT)
		return (TRUE);
	if (flag_output)
		*flag_output = 1;
	if ((*n)->redir[(*n)->f] == REDIRECT_OUT)
		fd_out = open((*n)->file[(*n)->f], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if ((*n)->redir[(*n)->f] == APPEND_OUT)
		fd_out = open((*n)->file[(*n)->f], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		clse_n_qit(NULL, p, (*n)->file[(*n)->f]);
		return (FALSE);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		clse_n_qit(&fd_out, p, (*n)->file[(*n)->f]);
		return (FALSE);
	}
	safe_close(&fd_out);
	return (TRUE);
}

void	msg_not_executable(char *str)
{
	ft_putstr_fd(str, 2);
	ft_putendl_fd(": not executable", 2);
}

void	first_child(t_pipex *p, PARSER **nodes, t_mega_struct *mini)
{
	int	flag_output;

	(*nodes)->f = 0;
	flag_output = 0;
	safe_close(&p->pipefd[0]);
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
			(clse_n_qit(NULL, p, "pipe"),
				free_exit(p, mini, EXIT_FAILURE));
	}
	safe_close(&p->pipefd[1]);
	if (!(*nodes)->command)
		free_exit(p, mini, EXIT_SUCCESS);
	if (!execute(nodes, p, mini))
		(msg_not_executable((*nodes)->command[0]), free_exit(p, mini, 126));
}

void	inter_child(t_pipex *p, PARSER **nodes, t_mega_struct *mini)
{
	int	flag_output;

	(*nodes)->f = 0;
	flag_output = 0;
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
		(safe_close(&p->prev_fd), clse_n_qit(NULL, p, "pipe"), free_exit(p, mini, 1));
	(safe_close(&p->pipefd[0]), safe_close(&p->prev_fd));
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
			(clse_n_qit(NULL, p, "pipe"), free_exit(p, mini, 1));
	}
	safe_close(&p->pipefd[1]);
	if (!(*nodes)->command)
		free_exit(p, mini, EXIT_SUCCESS);
	if (!execute(nodes, p, mini))
		(msg_not_executable((*nodes)->command[0]), free_exit(p, mini, 126));
}

void	last_child(t_pipex *p, PARSER **nodes, t_mega_struct *mini)
{
	(*nodes)->f = 0;
	safe_close(&p->pipefd[1]);
	if (dup2(p->prev_fd, STDIN_FILENO) == -1)
	{
		clse_n_qit(NULL, p, "pipe");
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
			safe_close(&p->prev_fd);
			p->prev_fd = -1;
		}
		if (p->i < p->nb_cmd -1)
		{
			safe_close(&p->pipefd[1]);
			p->prev_fd = p->pipefd[0];
		}
		else
			safe_close(&p->pipefd[0]);
	}
}

void	c_child(int signum)
{
	g_signal = signum;
	ft_putstr_fd("\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	safe_close_array(int **array, PARSER **node)
{
	int	i;

	if (!array || !*array)
		return ;
	i = 0;
	while (i < (*node)->nb_heredoc)
	{
		if (array[i])
			safe_close(&array[i][0]);
		i++;
	}
}

void	create_process(t_pipex *p, PARSER **nodes, t_mega_struct *mini)
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
			safe_close_array((*nodes)->fd_heredoc, nodes);
	}
}

int	cpy_std(int *cpy_stdin, int *cpy_stdout)
{
	*cpy_stdin = dup(STDIN_FILENO);
	if (!*cpy_stdin)
	{
		perror("cpy_stdin");
		return (FALSE);
	}
	*cpy_stdout = dup(STDOUT_FILENO);
	if (!*cpy_stdout)
	{
		perror("cpy_stdout");
		return (FALSE);
	}
	return (TRUE);
}

int	restore_std(int *cpy_stdin, int *cpy_stdout)
{
	if (*cpy_stdout != -1)
	{
		if (dup2(*cpy_stdout, STDOUT_FILENO) == -1)
			perror("STDOUT");
		safe_close(cpy_stdout);
		*cpy_stdout = -1;
	}
	if (*cpy_stdin != -1)
	{
		if (dup2(*cpy_stdin, STDIN_FILENO) == -1)
		{
			perror("STDIN");
			return (FALSE);
		}
		safe_close(cpy_stdin);
		*cpy_stdin = -1;
	}
	if (*cpy_stdout != -1)
		return (FALSE);
	return (TRUE);
}

int	simpl_pro_input(PARSER *nod, int *fd_in, int *d)
{
	if (nod->redir[nod->f] == REDIRECT_IN)
	{
		*fd_in = open(nod->file[nod->f], O_RDONLY | 0644);
		if (*fd_in == -1)
			return (perror(nod->file[nod->f]), FALSE);
		if (dup2(*fd_in, STDIN_FILENO) == -1)
			return (safe_close(fd_in), perror(nod->file[nod->f]), FALSE);
		safe_close(fd_in);
	}
	else if (nod->redir[nod->f] == HEREDOC)
	{
		safe_close(&nod->fd_heredoc[*d][1]);
		if (dup2(nod->fd_heredoc[*d][0], STDIN_FILENO) == -1)
		{
			safe_close(&nod->fd_heredoc[*d][0]);
			perror(nod->file[nod->f]);
			return (FALSE);
		}
		safe_close(&nod->fd_heredoc[*d][0]);
		d++;
	}
	return (TRUE);
}

int	builtins(PARSER *nod, t_pipex *p, t_cpy *cpy, t_mega_struct *mini)
{
	int	fd_in;
	int	d;

	fd_in = -1;
	nod->f = 0;
	p->flag = 1;
	d = 0;
	while (nod->file && nod->file[nod->f] != NULL)
	{
		if (!simpl_pro_input(nod, &fd_in, &d))
			return (FALSE);
		if (!handle_output_redirection(&nod, p, NULL))
			return (FALSE);
		nod->f++;
	}
	if (!exec_builtin(nod, p, cpy, mini))
		nod->exit_code = 1;
	return (TRUE);
}

void	q_child(int signum)
{
	g_signal = signum;
	rl_on_new_line();
	rl_replace_line("", 0);
}

int	handle_builtin(PARSER *node, t_pipex *p, t_mega_struct *mini)
{
	t_cpy	cpy;

	cpy.cpy_stdin = -1;
	cpy.cpy_stdout = -1;
	if (cpy_std(&cpy.cpy_stdin, &cpy.cpy_stdout) == FALSE)
	{
		node->exit_code = 1;
		return (FALSE);
	}
	if (!builtins(node, p, &cpy, mini))
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

int	create_pipes(t_pipex *p, PARSER *node)
{
	if (p->nb_cmd > 1 && p->i < p->nb_cmd - 1)
	{
		if (pipe(p->pipefd) == -1)
		{
			perror("pipe");
			node->exit_code = 1;
			return (FALSE);
		}
	}
	return (TRUE);
}

int	handle_input(PARSER **nodes, t_pipex *p, t_mega_struct *mini)
{
	PARSER		*current;

	current = (*nodes);
	if (current && current->next == NULL && is_builtin(current))
		return (handle_builtin(current, p, mini));
	mini->begin = (*nodes);
	while (p->i < p->nb_cmd)
	{
		if (!create_pipes(p, current))
			return (FALSE);
		(signal(SIGINT, SIG_IGN), signal(SIGINT, c_child));
		signal(SIGQUIT, q_child);
		p->pid = fork();
		if (p->pid == -1)
		{
			(*nodes)->exit_code = 1;
			return (perror("fork"), close_pipefds(p), FALSE);
		}
		if (p->i == p->nb_cmd - 1)
			p->last_pid = p->pid;
		create_process(p, &current, mini);
		p->i++;
		current = current->next;
	}
	return (close_pipefds(p), ft_wait(p->last_pid, nodes));
}
