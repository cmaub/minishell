/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:53:14 by anvander          #+#    #+#             */
/*   Updated: 2024/11/15 16:07:14 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	exit_code;

void	ft_error(char *str)
{
	perror(str);
	exit_code = 1;
	exit(EXIT_FAILURE);
}

int	ft_error_int(char *str)
{
	perror(str);
	exit_code = 1;
	return (-1);
}

// if we try to close a fd that is already closed, it will return -1
// an unsuccessful close will return (Bad file descriptor) in errno
void	safe_close(int fd)
{
	// static	int	i = 0;
	if (fd != -1)
	{
		if (close(fd) == -1)
		{
			perror("close AC");
			exit (EXIT_FAILURE);
		}
		fd = -1;
	}
	// i++;
}
int		ft_size_list(PARSER **nodes)
{
	PARSER	*current;
	int	size;

	current = *nodes;
	size = 0;
	while (current != NULL)
	{
		size++;
		current = current->next;
	}
	return (size);
}

char	**copy_env(char **envp)
{
	char	**mini_env;
	int	i;

	i = 0;
	while (envp[i])
		i++;
	mini_env = ft_calloc(i + 1, sizeof(char *));
	if (!mini_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		mini_env[i] = ft_strdup(envp[i]);
		// dprintf(2, "mini_env[%d] = %s\n", i, mini_env[i]);
		if (!mini_env[i])
		{
			while (i >= 0)
			{
				free(mini_env[i]);
				i--;
			}
		}
		i++;
	}
	mini_env[i] = NULL;
	return (mini_env);
}

void	ft_init_struct(t_pipex *p, char **mini_env, PARSER *nodes)
{
	p->mini_env = mini_env;
	p->nb_cmd = ft_size_list(&nodes);
	p->i = 0;
	p->prev_fd = -1;
	p->pid = 0;
	p->last_pid = 0;
	p->exit = 0;
	p->flag = 0;
}

int	is_str(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!isalpha(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	check_open(int fd)
{
	if (fd == -1)
	{
		perror("open");
		exit (EXIT_FAILURE);
	}
}

int	no_envp(char **tab)
{
	int	i;

	i = 0;
	if (tab == NULL)
		return (1);
	else
	{
		while (tab[i] != NULL)
			i++;
		if (i == 0)
			return (1);
	}
	return (0);
}

void	get_lines(PARSER *nodes, int i, int d)
{
	char	*str;
	while (1)
	{
		write(1, "heredoc> ", 9);
		str = get_next_line(0);
		if (str == 0)
			break ;
		if (ft_strncmp(str, nodes->delimiter[d], ft_strlen(nodes->delimiter[d])) == 0
			&& str[ft_strlen(nodes->delimiter[d])] == '\n')
		{
			free(str);
			get_next_line(-42);
			break ;
		}
		write(nodes->fd_heredoc[i], str, ft_strlen(str));
		free(str);
	}
	safe_close(nodes->fd_heredoc[i]);
}

void    ft_close_error(int *fd, t_pipex *p, char *str)
{
	if (fd)
	{
	    close(*fd);
    	close(p->pipefd[1]);
    	close(p->pipefd[0]);
	exit_code = 1;
    	perror(str);
    	exit(EXIT_FAILURE);
	}
}

int    ft_wait(pid_t last_pid, PARSER **nodes)
{
	int        status;
	int        exit_status;
	PARSER	*current;
	pid_t    waited_pid;

	current = *nodes;
	exit_status = 0;
	waited_pid = wait(&status);
	while (waited_pid != -1)
	{
		waited_pid = wait(&status);
		if (current->redir_type_in && current->redir_type_in[current->i] == 4)
			unlink(current->infile[current->i]);
		if (current->next)
			current = current->next;
		dprintf(2, "waited_pid == %d\n", waited_pid);
		if (waited_pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);		
		}
	}
	if (WIFEXITED(status))
	{
		if (exit_code == 0)
			exit_status = WEXITSTATUS(status);
		else
			exit_status = exit_code;
	}
	exit_code = exit_status;
	return (exit_code);
}

