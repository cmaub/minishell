/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:53:14 by anvander          #+#    #+#             */
/*   Updated: 2024/11/05 15:46:31 by anvander         ###   ########.fr       */
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

void	ft_init_struct(t_pipex *p, int ac, char **envp, PARSER *nodes)
{
	p->ac = ac;
	p->envp = envp;
	// p->heredoc = NULL;
	p->nb_cmd = ft_size_list(&nodes);
	printf("p->nb_cmd = %d\n", p->nb_cmd);
	p->i = 0;
	p->prev_fd = -1;
	p->pid = 0;
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

// int	list_size(t_token *list)
// {
// 	t_token	*temp;
// 	int		size;

// 	temp = list;
// 	size = 0;
// 	while (temp->next != NULL)
// 	{
// 		size++;
// 		temp = temp->next;
// 	}
// 	return (size);
// }

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

void	get_lines(PARSER *nodes, int i)
{
	char	*str;
	while (1)
	{
		write(1, "heredoc> ", 9);
		str = get_next_line(0);
		if (str == 0)
			break ;
		if (ft_strncmp(str, nodes->delimiter[i], ft_strlen(nodes->delimiter[i])) == 0
			&& str[ft_strlen(nodes->delimiter[i])] == '\n')
		{
			free(str);
			get_next_line(-42);
			break ;
		}
		write(nodes->fd_heredoc[i], str, ft_strlen(str));
		free(str);
	}
	safe_close(nodes->fd_heredoc[i]);
	// free (str);
}

void    ft_close_error(int *fd, t_pipex *p, char *str)
{
	if (fd)
	    close(*fd);
    	close(p->pipefd[1]);
    	close(p->pipefd[0]);
    	perror(str);
    	exit(EXIT_FAILURE);
}

int    ft_wait(pid_t last_pid, PARSER **nodes)
{
    int        status;
    int        exit_code;
    PARSER	*current;
    pid_t    waited_pid;

	current = *nodes;
    exit_code = 0;
    while (waited_pid != -1)
    {
	waited_pid = wait(&status);
	dprintf(2, "current->redir_type_in[current->i] = %d\n", current->redir_type_in[current->i]);
	if (current->redir_type_in[current->i] == 4)
	{
		dprintf(2, "current->infile[current->i] = %s\n", current->infile[current->i]);
		unlink(current->infile[current->i]);
	}
	if (current->next)
		current = current->next;
	dprintf(2, "waited_pid == %d\n", waited_pid);
	if (waited_pid == last_pid)
	{
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);	
	}
    }
    dprintf(2, "fin de ft_wait (%s, %d)\n", __FILE__, __LINE__);
    return (exit_code);
}

