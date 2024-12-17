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

int	ft_error_int(char *str, PARSER *node)
{
	ft_putendl_fd(str, 2);
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
			return ;
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

// **** RETIRER ?

// char	**copy_env()
// {
// 	char	**new_tab;
// 	int	i;

// 	i = 0;

// 	new_tab = try_malloc(4 * sizeof(char *));
// 	if (!new_tab)
// 		return (NULL);
// 	new_tab[0] = getcwd(NULL, 0);
// 	new_tab[1] = NULL;
// 	//dprintf(2, "new_tab[0] = %s\n", new_tab[0]);
// 	return (new_tab);
// }

char	**copy_tab(char **tab)
{
	char	**new_tab;
	int	i;

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
			while (i >= 0)
			{
				free(new_tab[i]);
				new_tab[i] = NULL;
				i--;
			}
			return (NULL);
		}
		i++;
	}
	new_tab[i] = NULL;
	return (new_tab);
}

void	ft_init_struct(t_pipex *p, t_env **chained_env, PARSER *nodes)
{
	p->env_nodes = chained_env;
	p->nb_cmd = ft_size_list(&nodes);
	p->i = 0;
	p->d = 0;
	p->prev_fd = -1;
	p->pid = 0;
	p->last_pid = 0;
	p->exit = 0; //supprimer ?
	p->flag = 0;
	p->pipefd[0] = -1;
	p->pipefd[1] = -1;
}

// *** RETIRER ?
// int	is_str(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (!ft_isalpha(str[i]))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab || !(*tab))
		return;
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

void	close_error_and_free(int *fd, t_pipex *p, PARSER **nodes, char *str, int exit_c)//remplacer en passant mini ?
{
	if (fd)
		safe_close(fd);
	safe_close(&p->pipefd[1]);
	safe_close(&p->pipefd[0]);
	perror(str);
	if (p->env_nodes)
		free_t_env(p->env_nodes);
	free_pipex(&p);
	reset_node(nodes);
	exit(exit_c);
}

void	clse_n_qit(int *fd, t_pipex *p, char *str)
{
	if (fd)
		safe_close(fd);
	safe_close(&p->pipefd[1]);
	safe_close(&p->pipefd[0]);
	perror(str);
}

int ft_wait(pid_t last_pid, PARSER **nodes)
{
	int   status;
	int   status_code;
	pid_t waited_pid;
	PARSER *current;	

	status_code = 0;
	current = *nodes;
	if (current == NULL)
		return (0);
	while ((waited_pid = wait(&status)) != -1)
	{
		if (current && current->next)
			current = current->next;
		else
			current = *nodes;	
		if (waited_pid == last_pid)
		{
			if (WIFEXITED(status))
			{
				status_code = WEXITSTATUS(status);
				(*nodes)->exit_code = status_code;
			}
			else if (g_signal != 0)
			{
		 		status_code = 128 + g_signal;
				(*nodes)->exit_code = status_code;
				g_signal = 0;
			}
		}
	}		
	// if (*nodes && (*nodes)->exit_code != 0)
	// 	status_code = (*nodes)->exit_code;
	// else
	// {
	// 	dprintf(2, "*** (%s, %d), status_code = %d\n", __FILE__, __LINE__, status_code);
	// 	(*nodes)->exit_code = status_code;
	// 	status_code = (*nodes)->exit_code;
	// }
	return (signal(SIGINT, handle_c_signal), status_code);
	// return (signal(SIGINT, SIG_IGN), status_code);
	// return (status_code);

}

void	print_nodes_list(PARSER **nodes)
{
	int	index = 0;
	int	f;
	int	h;
	int	d;
	PARSER	*tmp;
	
	dprintf(2, "entree dans print_nodes_list\n");
	if (!nodes)
	{
		dprintf(2, "*** nodes est null (%s, %d)\n", __FILE__, __LINE__);
		return ;
	}
	if (!(*nodes)/* || !nodes*/)
	{
		dprintf(2, "*** *nodes est null (%s, %d)\n", __FILE__, __LINE__);
		return ;
	}
	tmp = (*nodes);
	while (index <= ft_size_list(nodes))
	{
		f = 0;
		h = 0;
		d = 0;
		while (f < 30 && tmp->file && tmp->file[f] != NULL)
		{
			printf("tmp->file[%d] = %s, type = %d\n", f, tmp->file[f], tmp->redir[f]);
			if (d < 30 && tmp->delimiter && tmp->delimiter[d] != NULL)
			{
				printf("tmp->delimiter = %s\n", tmp->delimiter[d]);
				d++;
			}
			f++;
		}
		while (h < 30 && tmp->command && tmp->command[h] != NULL)
		{
			printf("tmp->command[%d] = %s\n", h, tmp->command[h]);
			h++;
		}
		if (!tmp->next)
			break;
		tmp = tmp->next;
		index++;
		printf("\n");
	}
	printf("\n");
}

