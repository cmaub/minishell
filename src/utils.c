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


int	lstsize_t_env(t_env *lst)
{
	t_list	*temp;
	int		size;

	temp = lst;
	size = 0;
	while (temp != NULL)
	{
		size++;
		temp = temp->next;
	}
	return (size);
}

void	free_pipex(t_pipex **p)
{
	// if ((*p)->mini_env)
	// {
	// 	//dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
	// 	ft_free_tab((*p)->mini_env);
	// 	(*p)->mini_env = NULL;
	// 	//dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
	// }
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

char	**copy_env()
{
	char	**new_tab;
	int	i;

	i = 0;

	new_tab = try_malloc(4 * sizeof(char *));
	if (!new_tab)
		return (NULL);
	new_tab[0] = getcwd(NULL, 0);
	new_tab[1] = NULL;
	//dprintf(2, "new_tab[0] = %s\n", new_tab[0]);
	return (new_tab);
}

char	**copy_tab(char **tab)
{
	char	**new_tab;
	int	i;

	i = 0;
	while (tab[i])
		i++;
	new_tab = try_malloc((i + 1) * sizeof(char *));
	if (!new_tab)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		new_tab[i] = ft_strdup(tab[i]);
		if (!new_tab[i])
		{
			while (i >= 0)
			{
				free(new_tab[i]);
				new_tab[i] = NULL;
				i--;
			}
		}
		i++;
	}
	new_tab[i] = NULL;
	return (new_tab);
}

void	ft_init_struct(t_pipex *p, t_env **chained_env, PARSER *nodes)
{
	p->mini_env = copy_tab(env);
	// p->mini_env = env;
	p->env_nodes = chained_env;
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
	//dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
	if (!tab || !(*tab))
		return;
	//dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
	while (tab[i] && tab[i] != NULL)
	{
		// //dprintf(2, "tab[i] = %s\n", tab[i]);
		if (tab[i] != NULL)
		{
			free(tab[i]);
			tab[i] = NULL;
		}
		
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

	// //dprintf(2, "entree dans wait\n");
	status_code = 0;
	current = *nodes;
	if (current == NULL)
		return (0);
	while ((waited_pid = wait(&status)) != -1)
	{
		// dprintf(2, "exit_status = %d, (*nodes)->exit_code = %d, status_code = %d\n", exit_status, (*nodes)->exit_code, status_code);
		//dprintf(2, "WAITED_PID = %d\n", waited_pid);
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
				dprintf(2, "g_signal = %d\n", g_signal);
		 		status_code = 128 + g_signal;
				g_signal = 0;
			}
		}
	}		
	// dprintf(2, "apres boucle wait ** exit_status = %d, (*nodes)->exit_code = %d, status_code = %d\n", exit_status, (*nodes)->exit_code, status_code);
	
	// if (*nodes && (*nodes)->exit_code != 0)
	// 	status_code = (*nodes)->exit_code;
	if (*nodes)
	{
		(*nodes)->exit_code = status_code;
		status_code = (*nodes)->exit_code;
		// reset_node(nodes);
	}
	//dprintf(2, "sortie du wait\n");
	return (signal(SIGINT, handle_c_signal), status_code);
	// return (status_code);
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
