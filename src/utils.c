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

t_env	*ft_lst_env_last(t_env *lst)
{
	t_env	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	return (temp);
}

void	ft_lstadd_env_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	temp = ft_lst_env_last(*lst);
	temp->next = new;
}

int	lstsize_t_env(t_env **lst)
{
	t_env	*temp;
	int		size;

	temp = *lst;
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
	if (!p || !*p)
		return ;
	free(*p);
	*p = NULL;
}

void	ft_error_exit(char *str, int exit_c)
{
	perror(str);
	exit(exit_c);
}

int	ft_error_int(char *str, t_parser *node)
{
	ft_putendl_fd(str, 2);
	node->exit_code = 1;
	return (-1);
}

// if we try to close a fd that is already closed, it will return -1
// an unsuccessful close will return (Bad file descriptor) in errno
void	s_clse(int *fd)
{
	if (*fd != -1)
	{
		if (close(*fd) == -1)
			return ;
		*fd = -1;
	}
	else
		return ;
}

int	ft_size_list(t_parser **nodes)
{
	t_parser	*current;
	int		size;

	current = *nodes;
	size = 0;
	while (current != NULL)
	{
		size++;
		current = current->next;
	}
	return (size);
}

char	**copy_tab(char **tab)
{
	char	**new_tab;
	int		i;

	i = 0;
	if (!tab) //verifier verification
		return (NULL);
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
			free_array(new_tab);
			return (NULL);
		}
		i++;
	}
	new_tab[i] = NULL;
	return (new_tab);
}

// //tester le cas ou on doit supprimer l'allocation de new_tab
// char	**copy_tab(char **tab)
// {
// 	char	**new_tab;
// 	int		i;

// 	i = 0;
// 	if (!tab) //verifier verification
// 		return (NULL);
// 	while (tab[i])
// 		i++;
// 	new_tab = try_malloc((i + 1) * sizeof(char *));
// 	if (!new_tab)
// 		return (NULL);
// 	i = 0;
// 	while (tab[i])
// 	{
// 		new_tab[i] = ft_strdup(tab[i]);
// 		if (!new_tab[i])
// 		{
// 			while (--i >= 0)
// 			// {
// 				free(new_tab[i]);
// 			free(new_tab);
// 			return (NULL);
// 				// new_tab[i] = NULL;
// 				// i--;
// 			// }
// 			// return (NULL);
// 		}
// 		i++;
// 	}
// 	new_tab[i] = NULL;
// 	return (new_tab);
// }

void	ft_init_struct(t_pipex *p, t_env **chained_env, t_parser *nodes)
{
	p->env_n = chained_env;
	p->nb_cmd = ft_size_list(&nodes);
	p->i = 0;
	p->d = 0;
	p->prev_fd = -1;
	p->pid = 0;
	p->last_pid = 0;
	p->exit = 0;
	p->flag = 0;
	p->pipefd[0] = -1;
	p->pipefd[1] = -1;
}

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab || !(*tab))
		return ;
	while (tab[i] && tab[i] != NULL)
	{
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

void	clse_n_x(int *fd, t_pipex *p, char *str)
{
	if (fd)
		s_clse(fd);
	s_clse(&p->pipefd[1]);
	s_clse(&p->pipefd[0]);
	perror(str);
}

void	update_exit_code(int status, t_parser **nodes, int *status_code)
{
	if (WIFEXITED(status))
	{
		*status_code = WEXITSTATUS(status);
		(*nodes)->exit_code = *status_code;
	}
	else if (g_signal != 0)
	{
		*status_code = 128 + g_signal;
		(*nodes)->exit_code = *status_code;
		g_signal = 0;
	}
}

int	ft_wait(pid_t last_pid, t_parser **nodes)
{
	int		status;
	int		status_code;
	pid_t	waited_pid;
	t_parser	*current;	

	status_code = 0;
	current = *nodes;
	if (current == NULL)
		return (0);
	waited_pid = wait(&status);
	while (waited_pid != -1)
	{
		if (current && current->next)
			current = current->next;
		else
			current = *nodes;
		if (waited_pid == last_pid)
			update_exit_code(status, nodes, &status_code);
		waited_pid = wait(&status);
	}		
	return (signal(SIGINT, handle_c_signal), status_code);
}

// int	ft_wait(pid_t last_pid, t_parser **nodes)
// {
// 	dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
// 	int		status;
// 	int		status_code;
// 	pid_t	waited_pid;
// 	t_parser	*current;	

// 	status_code = 0;
// 	current = *nodes;
// 	if (current == NULL)
// 		return (0);
// 	while ((waited_pid = wait(&status)) != -1)
// 	{
// 		if (current && current->next)
// 			current = current->next;
// 		else
// 			current = *nodes;
// 		if (waited_pid == last_pid)
// 		{
// 			if (WIFEXITED(status))
// 			{
// 				status_code = WEXITSTATUS(status);
// 				(*nodes)->exit_code = status_code;
// 			}
// 			else if (g_signal != 0)
// 			{
// 				status_code = 128 + g_signal;
// 				(*nodes)->exit_code = status_code;
// 				g_signal = 0;
// 			}
// 		}
// 	}		
// 	return (signal(SIGINT, handle_c_signal), status_code);
// }

// void	print_nodes_list(t_parser **nodes)
// {
// 	int	index;
// 	int	f;
// 	int	h;
// 	int	d;
// 	t_parser	*tmp;
// 
// 	index = 0;
// 	if (!nodes)
// 	{
// 		//dprintf(2, "*** nodes est null (%s, %d)\n", __FILE__, __LINE__);
// 		return ;
// 	}
// 	if (!(*nodes)/* || !nodes*/)
// 	{
// 		//dprintf(2, "*** *nodes est null (%s, %d)\n", __FILE__, __LINE__);
// 		return ;
// 	}
// 	tmp = (*nodes);
// 	while (index <= ft_size_list(nodes))
// 	{
// 		f = 0;
// 		h = 0;
// 		d = 0;
// 		while (f < 30 && tmp->file && tmp->file[f] != NULL)
// 		{
// 			printf("tmp->file[%d] = %s,
					// type = %d\n", f, tmp->file[f], tmp->redir[f]);
// 			if (d < 30 && tmp->delimiter && tmp->delimiter[d] != NULL)
// 			{
// 				printf("tmp->delimiter = %s\n", tmp->delimiter[d]);
// 				d++;
// 			}
// 			f++;
// 		}
// 		while (h < 30 && tmp->command && tmp->command[h] != NULL)
// 		{
// 			printf("tmp->command[%d] = %s\n", h, tmp->command[h]);
// 			h++;
// 		}
// 		if (!tmp->next)
// 			break;
// 		tmp = tmp->next;
// 		index++;
// 		printf("\n");
// 	}
// 	printf("\n");
// }
