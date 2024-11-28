/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/28 15:11:14 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_status = 0;

int parserHasReachEnd(LEXER *input) 
{
	if (input->head != input->len)
		return (FALSE);
	return (TRUE);
}

int	fill_list_of_tokens(LEXER *L_input, t_token **list)
{
	if (!expr(L_input, list) || !parserHasReachEnd(L_input))
	{
		free(L_input);
		return (FALSE);
	}
	free(L_input);
	return (TRUE);
}

void check_and_free_new_node(PARSER *new_node)
{
	if (!new_node)
       	return;
	if ((!new_node->file && new_node->nb_file > 0) ||
        (!new_node->command && new_node->nb_command > 0) ||
        (!new_node->delimiter && new_node->nb_heredoc > 0) ||
        (!new_node->redir_type && new_node->nb_file > 0) ||
        (!new_node->fd_heredoc && new_node->nb_heredoc > 0))
	{
    		free(new_node->file);
    		free(new_node->command);
    		free(new_node->delimiter);
    		free(new_node->redir_type);
    		free(new_node->fd_heredoc);
    		free(new_node);
	}
}

void	free_new_node(PARSER *new_node)
{
	if (new_node)
    {
		if ((new_node->file && new_node->nb_file > 0))
			free(new_node->file);
		if (new_node->command && new_node->nb_command > 0)
			free(new_node->command);
		if (new_node->delimiter && new_node->nb_heredoc > 0)
			free(new_node->delimiter);
		if (new_node->redir_type && new_node->nb_file > 0)
			free(new_node->redir_type);
		if (new_node->fd_heredoc && new_node->nb_heredoc > 0)
			free(new_node->fd_heredoc);
		free(new_node);
	}
}

void	calloc_tab_of_node(PARSER *new_node)
{
	if (new_node->nb_file > 0)
	{
		new_node->file = try_malloc((new_node->nb_file + 1) * sizeof(char *));
		new_node->redir_type = try_malloc((new_node->nb_file) * sizeof(int));
	}
	if (new_node->nb_command > 0)
		new_node->command = try_malloc((new_node->nb_command + 3) * sizeof(char *));
	if (new_node->nb_heredoc > 0)
	{
		new_node->delimiter = try_malloc((new_node->nb_heredoc + 1) * sizeof(char *));
		new_node->fd_heredoc = try_malloc((new_node->nb_heredoc) * sizeof(int));
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
		return (NULL);
	new_var = ft_strdup(*mini_env - 1 + (ft_strlen(str) + 1));
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
	return (i);
}

char	*join_char(char c, char *tmp)
{
	char	single_char[2];

	single_char[0] = c;
	single_char[1] = '\0';
	return (ft_strjoin(tmp, single_char));
}

char	*print_exit_code(char *result, PARSER *new_node, int *index)
{
	char	*expand_result;

	if (exit_status != 130)
	{
		expand_result = ft_strdup(ft_itoa(new_node->exit_code));
	}
	else
	{
		expand_result = ft_strdup(ft_itoa(130));
		exit_status = 0;
	}
	new_node->exit_code = 0;
	*index += 2;
	return (ft_strjoin(result, expand_result));
}

char	*print_expand(char *str, int *index, char **mini_env)
{
	char	*expand_expr;
	char	*expand_result;

	expand_expr = isolate_expand(str, *index + 1);
	expand_result = return_var_from_env(expand_expr, mini_env);
	// if (!expand_expr)
	// 	return (NULL);
	*index += ft_strlen(expand_expr) + 1;
	free(expand_expr);
	if (expand_result != NULL)
		return (ft_strdup(expand_result));
	return (ft_strdup(""));
}

char	*process_unquoted(PARSER *new_node, char *str, int *index, char **mini_env)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	tmp = NULL;
	while (str[*index] != 39 && str[*index] != 34 && str[*index] != '\0')
	{
		if (str[*index] == '$' 
			&& (ft_isalpha(str[(*index) +1]) || str[(*index) +1] == '_' || str[(*index) + 1] == '?' 
			|| ((str[(*index) + 1]) == 39 && (str[(*index) + 2] == 39))))
		{
			if (str[(*index) + 1] && str[(*index) + 1] == '?')
				tmp = print_exit_code(result, new_node, index);
			else
				tmp = print_expand(str, &(*index), mini_env);
			result = ft_strjoin(result, tmp);
			free(tmp);
		}
		else
		{
			
			tmp = join_char(str[*index], result);
			free(result);
			result = tmp;
			if (result[ft_strlen(result) - 1] == '$' && (str[(*index) + 1] == 39 || str[(*index) + 1] == 34))
				result[ft_strlen(result) - 1] = '\0';
			(*index)++;
		}
	}
	return (result);
}

char	*process_single_quotes(char *str, int *index)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	tmp = NULL;
	(*index)++; // passer quote ouvrante
	while (str[*index] != 39 && str[*index] != '\0')
	{
		tmp = join_char(str[*index], result);
		free(result);
		result = tmp;
		(*index)++;
	}
	if (str[*index] == 39)
		(*index)++;
	return (result);
}

char	*process_double_quotes(PARSER *new_node, char *str, int *index, char **mini_env)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	tmp = NULL;
	(*index)++; // Passer quote ouvrante
	while (str[*index] != 34 && str[*index] != '\0')
	{
		if (str[*index] == '$' 
			&& (ft_isalpha(str[(*index) +1]) || str[(*index) +1] == '_' || str[(*index) + 1] == '?'))
		{
			if (str[(*index) + 1] && str[(*index) + 1] == '?')
				tmp = print_exit_code(result, new_node, index);
			else
				tmp = print_expand(str, &(*index), mini_env);
			result = ft_strjoin(result, tmp);
			free(tmp);
		}
		else
		{
			tmp = join_char(str[*index], result);
			free(result);
			result = tmp;
			(*index)++;
		}
	}
	if (str[*index] == 34)
		(*index)++;
	return (result);
}

char	*withdraw_quotes(PARSER *new_node, char *str, char **mini_env)
{
	int	start;
	char	*result;
	char	*tmp;

	start = 0;
	result = "";
	while (str[start] != '\0')
	{
		if (str[start] == 34)
			tmp = process_double_quotes(new_node, str, &start, mini_env);
		else if (str[start] == 39)
			tmp = process_single_quotes(str, &start);
		else
			tmp = process_unquoted(new_node, str, &start, mini_env);
		result = ft_strjoin(result, tmp);
		free (tmp);
	}
	return (result);
}

void	calculate_size_of_tab(t_token *cur, PARSER *new_node, char **mini_env)
{
	if (cur->type == REDIRECT_IN || cur->type == HEREDOC 
		|| cur->type == REDIRECT_OUT || cur->type == APPEND_OUT)
	{
		if (cur->type == HEREDOC)
			new_node->nb_heredoc++;
		if (cur->next != NULL)
			cur->next->value = withdraw_quotes(new_node, cur->next->value, mini_env);
		new_node->nb_file++;
	}
	else if (cur->type == ARGUMENT)
	{
		cur->value = withdraw_quotes(new_node, cur->value, mini_env);
		new_node->nb_command++;
	}
}

PARSER	*alloc_new_node(t_token *current, char **mini_env, int exit_code)
{
	t_token	*cur;
	PARSER		*new_node;

	cur = current;
	new_node = try_malloc(sizeof(PARSER));
	new_node->exit_code = exit_code;
	while (cur && cur->type != PIPEX)
	{
		calculate_size_of_tab(cur, new_node, mini_env);
		cur = cur->next;
	}
	calloc_tab_of_node(new_node);
	check_and_free_new_node(new_node);
	return (new_node);
}

void	handle_c_signal_heredoc(int signum)
{
	(void)signum;
	exit_status = 130;
	write(1, "\n", 1);
	exit(130);
	
}
int	loop_readline(char *delimiter, int fd_heredoc)
{
	char	*input;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		input = readline("heredoc> ");
		if (!input)
		{
			ft_putendl_fd("warning: here-document delimited by end-of-file", 2);
			close(fd_heredoc);
			return (0);
		}
		dprintf(2, "delimit = %s, input = %s\n", delimiter, input);	
		if (ft_strncmp(input, delimiter, ft_strlen(delimiter)) == 0/* && input[ft_strlen(delimiter)] == '\n'*/)
		{
			close(fd_heredoc);
			free(input);
			break;
		}
		else
		{
			ft_putendl_fd(input, fd_heredoc);			
		}
		free(input);
	}
	close(fd_heredoc);
	return (0);
}

int	create_heredoc(PARSER *new_node, t_token *current, int *f, int *d)
{
	int		index_heredoc;
	int		random;
	char	*name_heredoc;
	int	save_fd_heredoc;
	int	tmp_fd;
	int	pid;
	int status = 0;

	name_heredoc = ft_strdup("heredoc");
	index_heredoc = 0;
	random = 1;
	name_heredoc[ft_strlen(name_heredoc)] = index_heredoc + '0';
	name_heredoc[ft_strlen(name_heredoc) + 1] = '\0';
	while (access(name_heredoc, F_OK) == 0)
	{
		name_heredoc[ft_strlen(name_heredoc)] = (index_heredoc + random) + '0';
		name_heredoc[ft_strlen(name_heredoc) + 1] = '\0';
		random++;	
	}
	save_fd_heredoc = dup(new_node->fd_heredoc[*f]);
	signal(SIGINT, SIG_IGN);
	new_node->fd_heredoc[*f] = open(name_heredoc, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (new_node->fd_heredoc[*f] == -1)
		perror("open");
	
	new_node->delimiter[*d] = ft_strdup(current->next->value);
	pid = fork();
	if (pid == 0)
	{
		// get_lines(new_node, *f, *d);
		loop_readline(new_node->delimiter[*d], new_node->fd_heredoc[*f]);
		exit(0);					
	}
	new_node->file[*f] = ft_strdup(name_heredoc);
	new_node->redir_type[*f++] = current->type;
	free(name_heredoc);
	name_heredoc = ft_strdup("heredoc");
	index_heredoc++;
	d++;
	wait(&status);
	signal(SIGINT, handle_c_signal_heredoc);
	if (status + 128 == 130)
	{
		tmp_fd = open(new_node->file[*f], O_WRONLY);
		safe_close(tmp_fd);
		exit_status = 130;
		unlink(new_node->file[*f]);
		return (-1);
	}
	return (0);
}

void	add_null_to_tab(PARSER *new_node, int f, int d, int cmd)
{
	if (f > 0)
		new_node->file[f] = NULL;
	if (d > 0)
		new_node->delimiter[d] = NULL;
	if (cmd > 0)
		new_node->command[cmd] = NULL;
}


// void	free_tokens(t_token **tokens)
// {
// 	t_token	*next_token;
// 	t_token	*current;
	
// 	current = *tokens;
// 	while (current)
// 	{
// 		if ((current)->next)
// 			next_token = current->next;
// 		dprintf(2, "token->value before free = %s\n", (current)->value);
// 		free((current)->value);
// 		current->value = NULL;
// 		free(current);
// 		// *current = NULL;
// 		// dprintf(2, "token->value after free = %s\n", (*current)->value);
// 		if ((current)->next)
// 			current = next_token;// current = next_token;
// 		else
// 			break;
// 	}
// 	*tokens = NULL;
// }

void	free_tokens(t_token *tokens)
{
	t_token	*next;
	
	while (tokens)
	{
		next = tokens->next;
		if (tokens->value)
		{
			free((tokens)->value);
			tokens->value = NULL;
		}
		free(tokens);
		tokens = next;
	}
}

int	create_nodes(t_token **tokens, PARSER **nodes, char **mini_env, int exit_code)
{
	t_token	*current;
	PARSER		*new_node;
	int		cmd;
	int		d;
	int		f;

	current = *tokens;
	while (current)
	{
		cmd = 0;
		d = 0;
		f = 0;
		new_node = alloc_new_node(current, mini_env, exit_code);
		if (!new_node)
		{
			free_tokens(*tokens);
			return (-1);
		}
		while (current && current->type != PIPEX)
		{
			if (current->type == REDIRECT_IN && current->next->value != NULL)
			{
				new_node->file[f] = ft_strdup(current->next->value);
				new_node->redir_type[f++] = current->type;
			}
			else if (current->type == HEREDOC && current->next->value != NULL)
				if (create_heredoc(new_node, current, &f, &d) == -1)
				{
					dprintf(2, "create heredoc = -1\n");
					free_tokens(*tokens);
					return (-1);
				}
			if ((current->type == REDIRECT_OUT || current->type == APPEND_OUT)
					&& current->next->value != NULL)
			{
					new_node->file[f] = ft_strdup(current->next->value);
					new_node->redir_type[f++] = current->type;
			}
			else if (current->type == ARGUMENT && current->value != NULL)
					new_node->command[cmd++] = ft_strdup(current->value);
			current = current->next;
		}
		add_null_to_tab(new_node, f, d, cmd);
		add_new_node(nodes, new_node);
		if (current && current->type == PIPEX)
			current = current->next;
		// check_and_free_new_node(new_node);
	}
	free_tokens(*tokens);
	return (0);
}

LEXER	*ft_init_lexer_input()
{
	LEXER	*new_input;
	
	new_input = try_malloc(sizeof(LEXER));
	new_input->data = NULL;
	new_input->len = 0;
	new_input->head = 0;
	return (new_input);
}

void handle_c_signal(int signum)
{
	(void)signum;
	// dprintf(2, "handle signal dans parent\n");
	ft_putstr_fd("\n", 2);
	 // // Réinitialise l'affichage de la ligne d'entrée
	// Indique que la ligne doit être recalculée
	rl_on_new_line();
	rl_replace_line("", 0);
	// rl_done = 1;
	// // Affiche une nouvelle ligne et le prompt
	rl_redisplay();	
	exit_status = 130;
}

int		main(int argc, char **argv, char **env)
{
	(void)argv;
	char		*str_input;
	LEXER		*L_input = NULL;
	t_token		*tokens = NULL;
	PARSER		*nodes = NULL;
	t_pipex	*p = NULL;
	char		**mini_env;
	int			exit_code = 0;

	mini_env = copy_env(env);
	if (argc >= 1)
	{
		while (1)
		{
			L_input = ft_init_lexer_input();
			if (!L_input)
			{
				write(1, "fatal: out of memory\n", 21);
				break;
			}
			tokens = NULL;
			nodes = NULL;
			signal(SIGINT, handle_c_signal);
			signal(SIGQUIT, SIG_IGN);
			str_input = readline("\001\033[36;1m\002minishell ➜ \001\033[0m\002");
			// str_input = readline("minishell ➜ ");
			if (!str_input)
			{
				ft_putstr_fd("exit\n", 1);
				rl_clear_history();
				free(L_input);
				break;
			}
			if (str_input && str_input[0])
				add_history(str_input);
			L_input->data = str_input;
			L_input->len = ft_strlen(str_input);
			if (str_input[0] == '\0')
			{
			}
			else if (!fill_list_of_tokens(L_input, &tokens))
			{
				ft_putendl_fd("syntax error", 2);
				exit_code = 2;
				// free_tokens(tokens);
			}
			else
			{
				if (create_nodes(&tokens, &nodes, mini_env, exit_code) == 0)
				{								
					// print_tokens_list(&tokens);
					// dprintf(2, "taille de list %d\n\n", ft_size_list(&nodes));
					// print_nodes_list(&nodes);
					p = try_malloc(sizeof(*p));
					ft_init_struct(p, mini_env, nodes);
					handle_input(&nodes, p);
					mini_env = p->mini_env;
					if (nodes)
						exit_code = nodes->exit_code;
					free(p);
				}
				reset_node(&nodes);

				//str_input deplace
			}
			// rl_replace_line("", 0);
			// rl_redisplay();
			free(str_input);
		}
	}
	ft_free_tab(mini_env);
	return (0);
}

// check history