/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:55:51 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 19:01:16 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
//
// void	print_t_env(t_env **mini_env)
// {
// 	t_env	*current;
//
// 	if (!mini_env || !*mini_env)
// 	{
// 	}
// 	current = *mini_env;
// 	while (current != NULL)
// 		current = current->next;
// }
//
// void	print_nodes_list(t_parser **nodes)
// {
// 	int	index = 0;
// 	int	f;
// 	int	h;
// 	int	d;
// 	t_parser	*tmp;
//
// 	dprintf(2, "entree dans print_nodes_list\n");
// 	if (!nodes)
// 	{
// 		dprintf(2, "*** nodes est null (%s, %d)\n", __FILE__, __LINE__);
// 		return ;
// 	}
// 	if (!(*nodes)/* || !nodes*/)
// 	{
// 		dprintf(2, "*** *nodes est null (%s, %d)\n", __FILE__, __LINE__);
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
// 			printf
// ("tmp->file[%d] = %s, type = %d\n", f, tmp->file[f], tmp->redir[f]);
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
