/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:53:14 by anvander          #+#    #+#             */
/*   Updated: 2024/10/28 14:35:22 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

// if we try to close a fd that is already closed, it will return -1
// an unsuccessful close will return (Bad file descriptor) in errno
void	safe_close(int fd)
{
	static	int	i = 0;
	if (fd != -1)
	{
		if (close(fd) == -1)
		{
			perror("close AC");
			exit (EXIT_FAILURE);
		}
		fd = -1;
	}
	i++;
}

void	ft_init_struct(t_pipex *p, int ac, t_token **token, char **envp)
{
	p->ac = ac;
	p->token = *token;
	p->envp = envp;
	p->heredoc = NULL;
	p->nb_cmd = 0;
	// if (ft_strncmp(token->type, "here_doc", 9) == 0)
	// 	p->nb_cmd--;
	p->i = 0;
	p->prev_fd = -1;
	p->pid = 0;
	// p->pipefd[0] = -1;
	// p->pipefd[1] = -1;
	p->last_pid = 0;
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

int	list_size(t_token *list)
{
	t_token	*temp;
	int		size;

	temp = list;
	size = 0;
	while (temp != NULL)
	{
		size++;
		temp = temp->next;
	}
	return (size);
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

void	get_lines(t_token *current, int fd_heredoc)
{
	char	*str;
	
	dprintf(2, "current->value %s\n", current->value);
	while (1)
	{
		write(1, "pipe heredoc>", 13);
		str = get_next_line(0);
		if (!str)
			break ;
		if ((ft_strncmp(str, current->value, ft_strlen(current->value)) == 0 && str[ft_strlen(current->value)] == '\n'))
		{
			get_next_line(-42);
			break ;
		}
		write(fd_heredoc, str, ft_strlen(str));
		free(str);
	}
	safe_close(fd_heredoc);
	free(str);
}

void    ft_close_error(int *fd, t_pipex *p, char *str)
{
    close(*fd);
    close(p->pipefd[1]);
    close(p->pipefd[0]);
    perror(str);
    exit(EXIT_FAILURE);
}

int    ft_wait(pid_t last_pid)
{
    int        status;
    int        exit_code;
    pid_t    waited_pid;

    exit_code = 0;
    while (true)
    {
        waited_pid = waitpid(-1, &status, 0);
        {
            if (waited_pid == -1)
                break ;
        }
        if (waited_pid == last_pid)
        {
            if (WIFEXITED(status))
                exit_code = WEXITSTATUS(status);
        }
    }
    return (exit_code);
}

