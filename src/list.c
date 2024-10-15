/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:00:17 by anvander          #+#    #+#             */
/*   Updated: 2024/10/15 17:01:02 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstadd(t_data **list, t_data *new)
{
	t_data    *current;

	if (!list || !new)
		return ;
	if (*list == NULL)
	{
		*list = new;
		return ;
	}
	current = *list;
	while (current->next != NULL)
		current = current->next;
	current->next = new;
}
	
t_data	*create_node(char *input)
{
	t_data	*new;

	new = malloc (sizeof(t_data));
	if (!new)
		return (NULL);
	new->value = input;
	new->next = NULL;
	return (new);
}

void	print_list(t_data *list)
{
	while (list != NULL)
	{
		printf("[%s]\n", list->value);
		list = list->next;
	}
}

