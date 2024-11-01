/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:04:02 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/01 17:59:51 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	create_process(t_pipex *p, PARSER *nodes)
// {
// 	if(p->pid == -1)
// 		ft_error("fork")
// 	else if(p->pid == 0)
// 	{
// 		if(p->i == 0)
// 			first_child
// 		else if (p->i < )
// 	}
		
// }

int	handle_input(PARSER *nodes, char **envp, int ac)
{
	t_pipex	*p;
	PARSER		*current;
	
	current = nodes; //il semblerait qu'il y ait un premier noeud vide donc on passe direct a next
	p = malloc(sizeof(*p));
	if (!p)
		return (free (nodes), free(current), 0);
	 // penser a free
	ft_init_struct(p, ac, envp, current);
	// while (p->i <= current->index)
	// {
	// 	if(p->i < current->index)
	// 	{
	// 		if (pipe(p->pipefd) == -1)
	// 			ft_error("pipe");
	// 	}
	// 	p->pid = fork();
	// 	if (p->i == current->index)
	// 		p->last_pid = p->pid;
	// 	// create_process(p, current);
	// 	p->i++;
	// 	current = current->next;
	// 	printf("current->index = %d\n", current->index);
	// }
	return (ft_wait(p->last_pid));
}