/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/15 16:04:49 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_nb_quote(char *input, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == c)
			count++;
		i++;
	}
	return (count);
}

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

t_data	*fill_list(char *input)
{
	int	i;
	int	start;
	int	end;
	t_data *list;
	t_data *new_node;
	
	i = 0;
	list = NULL;
	while (input[i] != '\0')
	{
		start = i;
		while (input[i] != 34 && input[i] != 39 && input[i] != 32 && input[i])
		{
			i++;
			// if (input[i] == 34 || input[i] == 39)
			// {
				
			// }	
		}
		end = i;
		if (end > start)
		{
			new_node = create_node(ft_substr(input, start, end - start));
			ft_lstadd(&list, new_node);
		}
		if (input[i] != '\0')
			i++;
	}
	return (list);
}

int		main(int argc, char **argv)
{
	(void)argv;
	char	*input;
	t_data	*tokens;

	if (argc >= 1)
	{
		while (1)
		{
			input = readline("~$");
			if (!input)
				break ;
			if (ft_strnstr(input, "exit", ft_strlen(input)))
				break ;
			if (input)
				add_history(input);
			printf("Ligne tapee %s\n", input);
			tokens = fill_list(input);
			print_list(tokens);
			free (input);
		}
	}
	return (0);
}