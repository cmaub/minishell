/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   utils.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: anvander < anvander@student.42.fr >		+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/16 15:53:14 by anvander		  #+#	#+#			 */
/*   Updated: 2024/11/25 11:38:55 by anvander		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

void	free_pipex(t_pipex **p)
{
	if ((*p)->mini_env)
	{
		ft_free_tab((*p)->mini_env);
		(*p)->mini_env = NULL;
	}
	free(*p);
	*p = NULL;
}

void	ft_error_exit(char *str, int exit_c)
{
	perror(str);
	exit(exit_c);
}

int	ft_error_int(char *str, PARSER *node)
{
	perror(str);
	node->exit_code = 1;
	return (-1);
}

// if we try to close a fd that is already closed, it will return -1
// an unsuccessful close will return (Bad file descriptor) in errno
void	safe_close(int *fd)
{
	if (*fd != -1)
	{
		if (close(*fd) == -1)
		{
			// perror("close2 AC");
			return ;
		}
		*fd = -1;
	}
	else
		return ;
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

char	**copy_tab(char **envp)
{
	char	**mini_env;
	int	i;

	i = 0;
	while (envp[i])
		i++;
	mini_env = try_malloc((i + 1) * sizeof(char *));
	if (!mini_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		mini_env[i] = ft_strdup(envp[i]);
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

void	ft_init_struct(t_pipex *p, char **env, PARSER *nodes)
{
	
	p->mini_env = env;
	p->nb_cmd = ft_size_list(&nodes);
	p->i = 0;
	p->prev_fd = -1;
	p->pid = 0;
	p->last_pid = 0;
	p->exit = 0; //supprimer ?
	p->flag = 0;
	p->pipefd[0] = -1;
	p->pipefd[1] = -1;
}

int	is_str(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isalpha(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab || !(*tab))
		return;
	while (tab[i] && tab[i] != NULL)
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = NULL;
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
void	close_error_and_free(int *fd, t_pipex *p, PARSER **nodes, char *str, int exit_c)
{
	if (fd)
		safe_close(fd);
	safe_close(&p->pipefd[1]);
	safe_close(&p->pipefd[0]);
	perror(str);
	free_pipex(&p);
	reset_node(nodes);
	exit(exit_c);
}
void	ft_close_error_no_exit(int *fd, t_pipex *p, PARSER **nodes, char *str)
{
	if (fd)
		safe_close(fd);
	safe_close(&p->pipefd[1]);
	safe_close(&p->pipefd[0]);
	perror(str);
	free_pipex(&p);
	reset_node(nodes);
}

int ft_wait(pid_t last_pid, PARSER **nodes)
{
	int   status;
	int   status_code;
	pid_t waited_pid;
	PARSER *current;	

	dprintf(2, "entree dans wait\n");
	status_code = 0;
	current = *nodes;
	if (current == NULL)
		return (0);
	while ((waited_pid = wait(&status)) != -1)
	{
		dprintf(2, "WAITED_PID = %d\n", waited_pid);
		if (current && current->next)
			current = current->next;
		else
			current = *nodes;	
		if (waited_pid == last_pid)
		{
			if (WIFEXITED(status))
			{
				
				status_code = WEXITSTATUS(status);
				// exit_status = 0;
			}
			if (g_signal != 0)
			{
		 		status_code = 128 + g_signal;
				g_signal = 0;
			}
		}
	}	
	// if (*nodes && (*nodes)->exit_code != 0)
	// 	status_code = (*nodes)->exit_code;
	if (*nodes)
		(*nodes)->exit_code = status_code;
	return (signal(SIGINT, handle_c_signal), (*nodes)->exit_code);
}

		

// void	get_lines(PARSER *nodes, int i, int d)
// {
// 	char	*str;
// 	while (1)
// 	{
// 		write(1, "heredoc> ", 9);
// 		str = get_next_line(0);
// 		if (str == 0)
// 			break ;
// 		if (ft_strncmp(str, nodes->delimiter[d], ft_strlen(nodes->delimiter[d])) == 0
// 			&& str[ft_strlen(nodes->delimiter[d])] == '\n')
// 		{
// 			free(str);
// 			get_next_line(-42);
// 			break ;
// 		}
// 		write(nodes->fd_heredoc[i], str, ft_strlen(str));
// 		free(str);
// 	}
// 	safe_close(nodes->fd_heredoc[i]);
// }
