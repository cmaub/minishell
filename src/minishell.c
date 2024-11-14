/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/14 00:07:12 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fill_list_of_tokens(LEXER *L_input, t_token **list)
{
	if (!expr(L_input, list))
		return (FALSE);
	return (TRUE);
}

void free_new_node(PARSER *new_node)
{
	if (!new_node)
       	return;
	if ((!new_node->infile && new_node->nb_infile > 0) ||
        (!new_node->outfile && new_node->nb_outfile > 0) ||
        (!new_node->command && new_node->nb_command > 0) ||
        (!new_node->delimiter && new_node->nb_heredoc > 0) ||
        (!new_node->redir_type_in && new_node->nb_infile > 0) ||
        (!new_node->redir_type_out && new_node->nb_outfile > 0) ||
        (!new_node->fd_heredoc && new_node->nb_heredoc > 0))
	{
    		free(new_node->infile);
    		free(new_node->outfile);
    		free(new_node->command);
    		free(new_node->delimiter);
    		free(new_node->redir_type_in);
    		free(new_node->redir_type_out);
    		free(new_node->fd_heredoc);
    		free(new_node);
	}
}

char	*return_var_from_env(char *str, char **mini_env)
{
	char	*new_var;

	new_var = NULL;
	str = ft_strjoin(str, "=");
	while (*mini_env && ft_strnstr(*mini_env, str, ft_strlen(str)) == NULL)
		mini_env++;
	if (*mini_env == NULL)
	{
		return (NULL);
	}
	new_var = ft_strdup(*mini_env + (ft_strlen(str) + 1));
	return (new_var);
}

char	*isolate_expand(char *str, int index)
{
	int	i;

	i = 0;
	while (str[index + i] != '\0')
	{
		if (str[index + i] == 39 || str[index + i] == 34 || str[index + i] == 32)
			break;
		i++;
	}
	return (ft_substr(str, index, i));
}

void	calloc_tab_of_node(PARSER *new_node)
{
	if (new_node->nb_infile > 0)
		new_node->infile = ft_calloc(new_node->nb_infile + 1, sizeof(char *));
	if (new_node->nb_outfile > 0)
		new_node->outfile = ft_calloc(new_node->nb_outfile + 1, sizeof(char *));
	if (new_node->nb_command > 0)
		new_node->command = ft_calloc(new_node->nb_command + 3, sizeof(char *));
	if (new_node->nb_heredoc > 0)
		new_node->delimiter = ft_calloc(new_node->nb_heredoc + 1, sizeof(char *));
	if (new_node->nb_infile > 0)
		new_node->redir_type_in = ft_calloc(new_node->nb_infile, sizeof(int));
	if (new_node->nb_outfile > 0)
		new_node->redir_type_out = ft_calloc(new_node->nb_outfile, sizeof(int));
	if (new_node->nb_heredoc > 0)
		new_node->fd_heredoc = ft_calloc(new_node->nb_heredoc, sizeof(int));
}

int	search_closing_quote(char *str, char c)
{
	int	i;

	i = 1;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	dprintf(2, "i = %d\n", i);
	return (i);
}

char	*join_char(char c, char *tmp)
{
	char	single_char[2];

	single_char[0] = c;
	single_char[1] = '\0';
	return (ft_strjoin(tmp, single_char));
}

char	*process_unquoted(char *str, int *index, char **mini_env)
{
	char	*result;
	char	*expand_expr;
	char	*expand_result;

	result = "";
	while (str[*index] != 39 && str[*index] != 34 && str[*index] != '\0')
	{
		if (str[*index] == '$' && str[*index + 1] != 39)
		{
			if (str[(*index) + 1] && str[(*index) + 1] == '?')
			{
				expand_expr = ft_strdup("?");
				expand_result = ft_strdup("0");
				result = ft_strjoin(result, expand_result);
			}
			else
			{
				expand_expr = isolate_expand(str, *index + 1);
				expand_result = return_var_from_env(expand_expr, mini_env);
				if (expand_result != NULL)
					result = ft_strjoin(result, expand_result);
			}
			*index += ft_strlen(expand_expr) +1;
		}
		else
		{
			result = join_char(str[*index], result);
			(*index)++;
		}
	}
	return (result);
}

char	*process_single_quotes(char *str, int *index)
{
	char	*result;

	result = "";
	(*index)++; // passer quote ouvrante
	while (str[*index] != 39 && str[*index] != '\0')
	{
		result = join_char(str[*index], result);
		(*index)++;
	}
	if (str[*index] == 39)
		(*index)++;
	return (result);
}

char	*process_double_quotes(char *str, int *index, char **mini_env)
{
	char	*result;
	char	*expand_expr;
	char	*expand_result;

	result = "";
	(*index)++; // Passer quote ouvrante
	while (str[*index] != 34 && str[*index] != '\0')
	{
		if (str[*index] == '$' && str[*index + 1] != 39)
		{
			if (str[(*index) + 1] && str[(*index) + 1] == '?')
			{
				expand_expr = ft_strdup("?");
				expand_result = ft_strdup("0");
				result = ft_strjoin(result, expand_result);
			}
			else
			{
				expand_expr = isolate_expand(str, *index + 1);
				expand_result = return_var_from_env(expand_expr, mini_env);
				if (expand_result != NULL)
					result = ft_strjoin(result, expand_result);
			}
			*index += ft_strlen(expand_expr) +1;
		}
		else
		{
			result = join_char(str[*index], result);
			(*index)++;
		}
	}
	if (str[*index] == 34)
		(*index)++;
	return (result);
}

char	*withdraw_quotes(char *str, char **mini_env)
{
	int	start;
	char	*result;

	start = 0;
	result = "";
	while (str[start] != '\0')
	{
		if (str[start] == 34)
			result = ft_strjoin(result, process_double_quotes(str, &start, mini_env));
		else if (str[start] == 39)
			result = ft_strjoin(result, process_single_quotes(str, &start));
		else
			result = ft_strjoin(result, process_unquoted(str, &start, mini_env));
	}
	return (result);
}

void	calculate_size_of_tab(t_token *cur, PARSER *new_node, char **mini_env)
{
	if (cur->type == REDIRECT_IN || cur->type == HEREDOC)
	{
		cur->next->value = withdraw_quotes(cur->next->value, mini_env);
		new_node->nb_infile++;
		if (cur->type == HEREDOC)
		{
			cur->next->value = withdraw_quotes(cur->next->value, mini_env);
		}
		new_node->nb_heredoc++;
	}
	if (cur->type == REDIRECT_OUT || cur->type == APPEND_OUT)
	{
		cur->next->value = withdraw_quotes(cur->next->value, mini_env);
		new_node->nb_outfile++;
	}
	else if (cur->type == ARGUMENT)
	{
		cur->value = withdraw_quotes(cur->value, mini_env);
		new_node->nb_command++;
	}
}

PARSER	*alloc_new_node(t_token *current, char **mini_env)
{
	t_token	*cur;
	PARSER		*new_node;

	cur = current;
	new_node = ft_calloc(1, sizeof(PARSER));
	if (!new_node)
		return (NULL);
	while (cur && cur->type != PIPEX)
	{
		calculate_size_of_tab(cur, new_node, mini_env);
		cur = cur->next;
	}
	calloc_tab_of_node(new_node);
	free_new_node(new_node);
	return (new_node);
}

void	create_nodes(t_token **tokens, PARSER **nodes, char **mini_env)
{
	t_token	*current;
	PARSER		*new_node;
	int		i;
	int		o;
	int		cmd;
	int		d;
	int		index_heredoc;
	char		*name_heredoc;

	name_heredoc = ft_strdup("heredoc");
	index_heredoc = 0;
	current = *tokens;
	while (current)
	{
		i = 0;
		o = 0;
		cmd = 0;
		d = 0;
		new_node = alloc_new_node(current, mini_env);
		if (!new_node)
			return ;
		while (current && current->type != PIPEX)
		{
			if (current->type == REDIRECT_IN && current->next->value != NULL)
			{
				new_node->infile[i] = ft_strdup(current->next->value);
				new_node->redir_type_in[i++] = current->type;
				// i++;
			}
			else if (current->type == HEREDOC && current->next->value != NULL)
			{
				name_heredoc[ft_strlen(name_heredoc)] = index_heredoc + '0';
				name_heredoc[ft_strlen(name_heredoc) + 1] = '\0';
				new_node->fd_heredoc[i] = open(name_heredoc, O_WRONLY | O_CREAT | O_APPEND, 0644);
				new_node->delimiter[d] = ft_strdup(current->next->value);
				get_lines(new_node, i, d);
				new_node->infile[i] = ft_strdup(name_heredoc);
				new_node->redir_type_in[i++] = current->type;
				free(name_heredoc);
				name_heredoc = ft_strdup("heredoc");
				index_heredoc++;
				// i++;
				d++;
			}
			if ((current->type == REDIRECT_OUT || current->type == APPEND_OUT)
					&& current->next->value != NULL)
			{
					new_node->outfile[o] = ft_strdup(current->next->value);
					new_node->redir_type_out[o++] = current->type;
			}
			else if (current->type == ARGUMENT && current->value != NULL)
					new_node->command[cmd++] = ft_strdup(current->value);
			current = current->next;
		}
		if (i > 0)
			new_node->infile[i] = NULL;
		if (d > 0)
			new_node->delimiter[d] = NULL;
		if (o > 0)
			new_node->outfile[o] = NULL;
		if (cmd > 0)
			new_node->command[cmd] = NULL;
		add_new_node(nodes, new_node);
		if (current && current->type == PIPEX)
			current = current->next;
	}
}


int	main(int argc, char **argv, char **env)
{
	(void)argv;
	char		*str_input;
	LEXER		*L_input = NULL;
	t_token		**tokens = NULL;
	PARSER		**nodes = NULL;
	char		**mini_env;

	str_input = NULL;
	mini_env = copy_env(env);
	if (argc >= 1)
	{
		while (1)
		{
			L_input = ft_calloc(10, sizeof(L_input)); // faire une fonction pour initialiser a part
			L_input->data = NULL;
			L_input->len = 0;
			L_input->head = 0;
			tokens = ft_calloc(1, sizeof(t_token *));
			nodes = ft_calloc(1, sizeof(PARSER *));

			// Utilisation de get_next_line au lieu de readline
			write(1, "~$ ", 3); // affiche le prompt
			str_input = get_next_line(0); // lit l'entrée standard

			if (!str_input)
				break;
			if (ft_strnstr(str_input, "quit", ft_strlen(str_input)))
				break;
			// if (str_input)
			// 	add_history(str_input); // conserver l'historique si readline est activé
			L_input->data = str_input;
			L_input->len = ft_strlen(str_input);

			if (!fill_list_of_tokens(L_input, tokens))
			{
				printf("input non valide\n");
				free(tokens);
				free(str_input);
				free(nodes);
				free(L_input);
			}
			else
			{
				create_nodes(tokens, nodes, mini_env);
				dprintf(2, "taille de list %d\n", ft_size_list(nodes));
				print_nodes_list(nodes);
				dprintf(2, "exit_code = %d\n", handle_input(nodes, mini_env, argc));

				free(tokens);
				free(str_input);
				free(nodes);
				free(L_input);
			}
		}
	}
	return (0);
}


// int		main(int argc, char **argv, char **env)
// {
// 	(void)argv;
// 	char		*str_input;
// 	LEXER		*L_input = NULL;
// 	t_token	**tokens = NULL;
// 	PARSER		**nodes = NULL;
// 	char		**mini_env;

// 	str_input = NULL;
// 	mini_env = copy_env(env);
// 	if (argc >= 1)
// 	{
// 		while (1)
// 		{
// 			L_input = ft_calloc(1, sizeof(L_input)); // faire une fonction pour initialiser a part
// 			L_input->data = NULL;
// 			L_input->len = 0;
// 			L_input->head = 0;
// 			tokens = ft_calloc(1, sizeof(t_token *));
// 			nodes = ft_calloc(1, sizeof(PARSER *));
// 			str_input = readline("~$");
// 			if (!str_input)
// 				break ;
// 			if (ft_strnstr(str_input, "quit", ft_strlen(str_input)))
// 				break ; // quand on appuie sur entree OU quand quote pas fermee puis fermee
// 			else
// 				rl_on_new_line();
// 			if (str_input)
// 				add_history(str_input);
// 			L_input->data = str_input;
// 			L_input->len = ft_strlen(str_input);
// 			if (!fill_list_of_tokens(L_input, tokens))
// 			{
// 				printf("input non valide\n");
// 				free(tokens);
// 				free(str_input);
// 				free(nodes);
// 				free(L_input);
// 			}
// 			else
// 			{
// 				create_nodes(tokens, nodes, mini_env);
// 				dprintf(2, "taille de list %d\n", ft_size_list(nodes));
// 				print_nodes_list(nodes);
// 				dprintf(2, "exit_code = %d\n", handle_input(nodes, mini_env, argc));
// 					// break ;

// 				free(tokens);
// 				free(str_input);
// 				free(nodes);
// 				free(L_input);
// 			}
// 		}
// 	}
// 	return (0);
// }
