/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/17 12:49:33 by cmaubert         ###   ########.fr       */
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

int	handle_quote(char *input, char quote, int *end, int *start, int *i)
{
	(*i)++;
	*start = *i;
	while (input[*i] != '\0' && input[*i] != quote)
		(*i)++;
	*end = *i;
	if (input[*i] == '\0')
		return (false);
	return (true);
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
			i++;
		end = i;
		if (input[i] == 34 || input[i] == 39)
			if (!handle_quote(input, input[i], &end, &start, &i))
				return (NULL);
		if (end > start)
		{
			new_node = create_node(ft_substr(input, start, end - start));
			ft_lstadd(&list, new_node);
		}
		if (input[i] != '\0')
			i++;
		else
			break ;
	}
	return (list);
}

int		main(int argc, char **argv, char **env)
{
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