/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/18 17:25:11 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	occurence_of_a_char(char *input, char c)
// {
// 	int	count;
// 	int	i;

// 	count = 0;
// 	i = 0;
// 	while (input[i] != '\0')
// 	{
// 		if (input[i] == c)
// 			count++;
// 		i++;
// 	}
// 	return (count);
// }

int	handle_quote(char *input, char quote, int *end, int *start, int *i)
{
	(*i)++;
	*start = *i;
	while (input[*i] != '\0' && input[*i] != quote)
		(*i)++;
	*end = *i;
	if (input[*i] == '\0' || *i == 1)    /* (*i == 1) si 2 quotes de meme nature se suivent - a revoir*/
	{
		printf("invalid quoting\n");
		return (false);
	}
	return (true);
}

int	give_type_to_token(t_token *token)
{
	if (!token || !token->value)
		return (TBD);
	else if (token->value[0] == '|' && !token->value[1])
		return (PIPE);
	else if (token->value[0] == '>' && !token->value[1])
		return (REDIRECT_OUT);
	else if (token->value[0] == '<' && !token->value[1])
		return (REDIRECT_IN);
	else if (token->value[0] == '>' && token->value[1] == '>' && !token->value[2])
		return (APPEND_OUT);
	else if (token->value[0] == '<' && token->value[1] == '<' && !token->value[2])
		return (HEREDOC);
	else if (!token->prev && token->value[0] != '-')
		return (COMMAND); /* Il faudra verifier qu'il s'agit bien d'une commande */
	if (token->prev)
	{
		if (token->prev->type == REDIRECT_IN || token->prev->type == REDIRECT_OUT
			|| token->prev->type == APPEND_OUT)
			return (FILENAME);
		else if (token->value[0] == '-' && ft_strlen(token->value) <= 3
			&& token->prev->type == COMMAND)
			return (OPTION);
		else if (token->prev->type == PIPE || token->prev->type == FILENAME)
			return (COMMAND);
		else if (token->prev->type == HEREDOC)
			return (DELIMITER);
	}
	/* Si TBD a ce stade, ca ne peut etre qu'une cmde un arg ou un file*/
	return (TBD);
}

/* INCOMPLETE */
int	is_input_valid(t_token *list)
{
	t_token	*current;

	current = list;
	/* l'input commence soit par une redirection soit par une commande */
	if (!current->prev && current->type > 4 && current->type != COMMAND)
		return (0);
	// if (current->type == ARGUMENT || current->type == PIPE)
	// 	return (0);
	if (list_size(list) == 1 && current->type != COMMAND) /* Attention certains caracteres fonctionnent cf tests 2 a 6 */
		return (0);
	while (current->next != NULL)
	{
		if (current->type == PIPE && current->next->type != COMMAND)
			return (0);
		if (current->type == OPTION && current->prev->type != COMMAND)
			return (0);
		current = current->next;
	}
	/* l'input ne pas terminer par une redirection ou un pipe */
	if (current->next == NULL && current->type < 6) 
		return (0);
	return (1);
}

t_token	*fill_list_of_tokens(char *input)
{
	int		i;
	int		start;
	int		end;
	int		token_index;
	t_token	*list;
	t_token	*new_node;
	
	i = 0;
	list = NULL;
	token_index = 0;
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
			new_node = create_new_token(ft_substr(input, start, end - start));
			add_new_token(&list, new_node);
			new_node->index = token_index;
			token_index++;
			if (input[start - 1] == 34 || input[start - 1] == 39)
				new_node->type = ARGUMENT;
			else
				new_node->type = give_type_to_token(new_node);
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
	(void)argv;
	char		*input;
	t_token	*tokens;

	if (argc >= 1)
	{
		while (1)
		{
			input = readline("~$");
			if (!input)
				break ;
			if (ft_strnstr(input, "exit", ft_strlen(input)))
				break ;
			else
				rl_on_new_line();
			if (input)
				add_history(input);
			tokens = fill_list_of_tokens(input);
			if (!tokens)
			{
				free (input);
				return (0);
			}
			print_tokens_list(tokens);
			if (!is_input_valid(tokens))
			{
				printf("invalid input\n");
				return (0);
			}
			printf("ICI\n");
			handle_input(tokens, env, argc);
			// write(2, "\n", 1);
			free (input);
		}
	}
	return (0);
}
