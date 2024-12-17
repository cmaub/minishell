/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 10:34:17 by anvander          #+#    #+#             */
/*   Updated: 2024/12/13 14:33:17 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	add_new_node(PARSER **nodes, PARSER *new_node)
{
	PARSER	*current;

	if (!nodes || !new_node)
		return ;
	if (!(*nodes))
	{
		*nodes = new_node;
		return ;
	}
	else
	{
		current = *nodes;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

void	calloc_tab_of_node(PARSER *new_node)
{
	int	i;

	i = 0;
	if (new_node->nb_file > 0)
	{
		new_node->file = try_malloc((new_node->nb_file + 1) * sizeof(char *));
		new_node->redir = try_malloc((new_node->nb_file) * sizeof(int));
	}
	if (new_node->nb_command > 0)
		new_node->command = try_malloc((new_node->nb_command + 3) * sizeof(char *));
	if (new_node->nb_heredoc > 0)
	{
		new_node->delimiter = try_malloc((new_node->nb_heredoc + 1) * sizeof(char *));
		new_node->fd_heredoc = try_malloc((new_node->nb_heredoc) * sizeof(int *));
		while (i < new_node->nb_heredoc)
		{
			new_node->fd_heredoc[i] = try_malloc(2 * sizeof(int));
			i++;
		}
	}
}

char	*return_var_from_env(char *str, t_env **chained_env)
{
	char	*new_var;
	char	*temp_str;
	t_env	*temp;

	temp = *chained_env;
	new_var = NULL;
	temp_str = ft_strjoin(str, "=");
	if (!temp_str)
		return (NULL);
	while (temp && ft_strnstr(temp->var, temp_str, ft_strlen(temp_str)) == NULL)
	{
			temp = temp-> next;
	}
	if (temp == NULL)
		return (free(temp_str), NULL);
	new_var = ft_strdup(temp->var - 1 + (ft_strlen(temp_str) + 1));
	if (!new_var)
		return (free(temp_str), NULL);
	free(temp_str);
	return (new_var);
}

char	*isolate_expand(char *str, int index)
{
	int	i;

	i = 0;
	while (str[index + i] != '\0')
	{
		if (!ft_isalnum(str[index + i]) && str[index + i] != '_')
			break ;
		
		i++;
	}
	return (ft_substr(str, index, i));
}

char	*join_char(char c, char *result)
{
	char	single_char[2];
	char	*tmp;

	single_char[0] = c;
	single_char[1] = '\0';
	tmp = ft_strjoin(result, single_char);
	free(result);
	if (!tmp)
		return (NULL);
	return (tmp);
}

char	*print_exit_code(PARSER *new_node, int *index)
{
	char	*expand_result;
	char	*itoa_result;

	itoa_result = ft_itoa(new_node->exit_code);
	if (!itoa_result)
		return (NULL);
	expand_result = ft_strdup(itoa_result);
	if (!expand_result)
			return (free(itoa_result), NULL);			
	new_node->exit_code = 0;
	*index += 2;
	return (free(itoa_result), expand_result);
}

char	*print_expand(char *str, int *index, t_env **chained_env)
{
	char	*expand_expr;
	char	*expand_result;
	char	*empty;

	empty = ft_strdup("");
	if (!empty)
		return (NULL);
	expand_expr = isolate_expand(str, *index + 1);
	expand_result = return_var_from_env(expand_expr, chained_env);
	*index += ft_strlen(expand_expr) + 1;
	free(expand_expr);
	if (expand_result != NULL)
		return (free(empty), expand_result);
	return (empty);
}

char	*expand_var(PARSER *node, char *str, int *i, t_env **env)
{
	char	*expand = NULL;

	if (str[(*i) + 1] && str[(*i) + 1] == '?')
	{
		expand = print_exit_code(node, i);
		if (!expand)
			return (NULL);
	}
	else
	{
		expand = print_expand(str, i, env);
		if (!expand)
			return (NULL);
	}
	return (expand);
}

int	is_expandable(char *str, int i, int flag)
{
	if (str[i] == '$')
	{
		if (flag == 2)
		{
			if (ft_isalpha(str[(i) +1]) 
				|| str[(i) +1] == '_' 
				|| str[(i) + 1] == '?')
				return (1);
		}
		else if (flag == 0)
		{
			if ((ft_isalpha(str[(i) +1]) 
				|| str[(i) +1] == '_' 
				|| str[(i) + 1] == '?' 
				|| ((str[(i) + 1]) == 39 
				&& (str[(i) + 2] == 39))))
				return (1);
		}
	}
	return (0);
}

char	*withdraw_nothing(PARSER *new_node, char *str, int *index, t_env **chained_env, char *result)
{
	char	*tmp;
	char	*tmp_result;
	char	*expand;

	if (is_expandable(str, *index, 0))
	{
		expand = expand_var(new_node, str, index, chained_env);
		if (!expand)
				return (NULL);
		tmp_result = ft_strjoin(result, expand);
		free(expand);
		// expand = NULL;
		if (!tmp_result)
			return (free(result), NULL);
		free(result);
		result = tmp_result;
	}
	else
	{
		tmp = join_char(str[*index], result);
		if (!tmp)
			return (free(result), NULL);
		result = tmp;
		(*index)++;
	}
	return (result);
}

char	*process_unquoted(PARSER *new_node, char *str, int *index, t_env **chained_env)
{
	char	*result;
	char	*tmp_result;
	char	*tmp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	tmp_result = NULL;
	tmp = NULL;
	while (str[*index] != 39 && str[*index] != 34 && str[*index] != '\0')
		result = withdraw_nothing(new_node, str, index, chained_env, result);
	return (result);
}

char	*process_single_quotes(char *str, int *index)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	tmp = NULL;
	(*index)++; // passer quote ouvrante
	while (str[*index] != 39 && str[*index] != '\0')
	{
		tmp = join_char(str[*index], result);
		if (!tmp)
			return (free(result), NULL);
		// free(result);
		result = tmp;
		(*index)++;
	}
	if (str[*index] == 39)
		(*index)++;
	return (result);
}

char	*withdraw_double(PARSER *new_node, char *str, int *index, t_env **chained_env, char *result)
{
	char	*tmp;
	char	*tmp_result;
	char	*expand;

	if (is_expandable(str, *index, 2))
	{
		expand = expand_var(new_node, str, index, chained_env);
		if (!expand)
				return (NULL);
		tmp_result = ft_strjoin(result, expand);
		free(expand);
		// expand = NULL;
		if (!tmp_result)
			return (free(result), NULL);
		free(result);
		result = tmp_result;
	}
	else
	{
		tmp = join_char(str[*index], result);
		if (!tmp)
			return (free(result), NULL);
		result = tmp;
		(*index)++;
	}
	return (result);
}

char	*process_double_quotes(PARSER *new_node, char *str, int *index, t_env **chained_env)
{
	char	*result;
	char	*tmp;
	char	*tmp_result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	tmp_result = NULL;
	tmp = NULL;
	(*index)++;
	while (str[*index] != 34 && str[*index] != '\0')
		result = withdraw_double(new_node, str, index, chained_env, result);
	if (str[*index] == 34)
		(*index)++;
	return (result);
}

char	*process_string(PARSER *new_node, char *str, int *start, t_env **chained_env)
{
	if (str[*start] == 34)
		return (process_double_quotes(new_node, str, start, chained_env));
	else if (str[*start] == 39)
		return (process_single_quotes(str, start));
	else
		return (process_unquoted(new_node, str, start, chained_env));
}

char	*withdraw_quotes(PARSER *new_node, char *str, t_env **chained_env)
{
	int	start;
	char	*result;
	char	*tmp_result;
	char	*tmp;

	start = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	// tmp_result = NULL;
	while (str[start] != '\0')
	{
		tmp = process_string(new_node, str, &start, chained_env);
		if (!tmp)
			return (free(result), free(tmp_result), NULL);
		tmp_result = ft_strjoin(result, tmp);
		free(tmp);
		// tmp = NULL ;
		if (!tmp_result)
			return (free(result), NULL);
		free(result);
		// result = NULL;
		result = tmp_result;
		tmp_result = NULL;
	}
	if (!result || !*result)
		return (free(result), NULL);
	return (result);
}

// char	*withdraw_quotes(PARSER *new_node, char *str, t_env **chained_env)
// {
// 	int	start;
// 	char	*result;
// 	char	*tmp_result;
// 	char	*tmp;

// 	start = 0;
// 	result = ft_strdup("");
// 	if (!result)
// 		return (NULL);
// 	tmp_result = NULL;
// 	while (str[start] != '\0')
// 	{
// 		if (str[start] == 34)
// 			tmp = process_double_quotes(new_node, str, &start, chained_env);
// 		else if (str[start] == 39)
// 			tmp = process_single_quotes(str, &start);
// 		else
// 			tmp = process_unquoted(new_node, str, &start, chained_env);
// 		if (!tmp)
// 			return (free(result), free(tmp_result), NULL);
// 		tmp_result = ft_strjoin(result, tmp);
// 		free(tmp);
// 		tmp = NULL ;
// 		if (!tmp_result)
// 			return (free(result), NULL);
// 		free(result);
// 		result = NULL;
// 		result = tmp_result;
// 		tmp_result = NULL;
// 	}
// 	if (!result || !*result)
// 		return (free(result), NULL);
// 	return (result);
// }

int	update_value_in_node(t_token **cur, PARSER *new_node, t_env **env)
{
	char	*tmp;

	tmp = NULL;
	tmp = ft_strdup(((*cur))->value);
	if (!tmp)
		return (FALSE);
	free((*cur)->value);
	(*cur)->value = NULL;
	(*cur)->value = withdraw_quotes(new_node, tmp, env);
	free(tmp);
	if (!(*cur)->value || (*cur)->value[0] == '\0')
	{
		return (FALSE);
	}
	return (TRUE);
}

// int	calculate_size_of_tab(t_token *cur, PARSER *new_node, t_env **chained_env)
// {
// 	char	*tmp;

// 	tmp = NULL;
// 	if (cur->type == REDIRECT_IN || cur->type == HEREDOC 
// 		|| cur->type == REDIRECT_OUT || cur->type == APPEND_OUT)
// 	{
// 		if (cur->type == HEREDOC)
// 			new_node->nb_heredoc++;
// 		if (cur->type)
// 		{
// 			tmp = ft_strdup(cur->value);
// 			if (!tmp)
// 				return (FALSE);
// 			free(cur->value);
// 			cur->value = NULL;
// 			cur->value = withdraw_quotes(new_node, tmp, chained_env);
// 			free(tmp);
// 			if (!cur->value)
// 				return (FALSE);
// 		}
// 		new_node->nb_file++;
// 	}
// 	else if (cur->type == ARGUMENT)
// 	{
// 		tmp = ft_strdup(cur->value);
// 		if (!tmp)
// 			return (FALSE);
// 		free(cur->value);
// 		cur->value = NULL;
// 		cur->value = withdraw_quotes(new_node, tmp, chained_env);	
// 		free(tmp);
// 		if (!cur->value || !*cur->value)
// 			return (FALSE);
// 		new_node->nb_command++;
// 	}
// 	return (TRUE);
// }

int	calculate_size_of_tab(t_token *cur, PARSER *new_node, t_env **chained_env)
{
	char	*tmp;

	tmp = NULL;
	if (cur->type == REDIRECT_IN || cur->type == HEREDOC 
		|| cur->type == REDIRECT_OUT || cur->type == APPEND_OUT)
	{
		if (cur->type == HEREDOC)
			new_node->nb_heredoc++;
		if (cur->type)
		{
			if (!update_value_in_node(&cur, new_node, chained_env))
				return (FALSE);
		}
		new_node->nb_file++;
	}
	else if (cur->type == ARGUMENT)
	{
		if (!update_value_in_node(&cur, new_node, chained_env))
			return (FALSE);
		new_node->nb_command++;
	}
	return (TRUE);
}

PARSER	*alloc_new_node(t_token *current, t_env **chained_env, int exit_code)
{
	t_token	*cur;
	PARSER		*new_node;

	cur = current;
	new_node = try_malloc(sizeof(PARSER));
	if (!new_node)
		return (NULL);
	new_node->exit_code = exit_code;
	while (cur && cur->type != PIPEX)
	{
		calculate_size_of_tab(cur, new_node, chained_env);
			// break ;
		cur = cur->next;
	}
	calloc_tab_of_node(new_node);
	check_and_free_new_node(new_node);
	return (new_node);
}

void	add_null_to_tab(PARSER *new_node, t_mega_struct *mini)
{
	if (mini->f > 0)
		new_node->file[mini->f] = NULL;
	if (mini->d > 0)
		new_node->delimiter[mini->d] = NULL;
	if (mini->cmd > 0)
		new_node->command[mini->cmd] = NULL;
}

void	handle_c_signal_heredoc(int signum)
{
	g_signal = signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_done = 1;
	close(STDIN_FILENO);
}

int	loop_readline(char *delimiter, int *fd_heredoc)
{
	char	*input;

	g_signal = 0;
	signal(SIGINT, handle_c_signal_heredoc);
	while (g_signal == 0)
	{
		input = readline("heredoc> ");
		if (g_signal == SIGINT)
			return (free(input), safe_close(fd_heredoc), -1);
		if (!input)
		{
			ft_putendl_fd("warning: here-document delimited by end-of-file", 2);
			return (safe_close(fd_heredoc), -1);
		}
		if (ft_strncmp(input, delimiter, ft_strlen(delimiter)) == 0)
		{
			safe_close(fd_heredoc);
			free(input);
			break;
		}
		else
			ft_putendl_fd(input, *fd_heredoc);
		free(input);
	}
	return (0);
}

int	create_heredoc(PARSER *new_node, t_token *current, int *f, int *d)
{
	int	fd;

	fd = dup(STDIN_FILENO);
	if (fd == -1)
		return (perror("dup"), FALSE);
	signal(SIGINT, SIG_IGN); //
	new_node->delimiter[*d] = ft_strdup(current->value);
	if (!new_node->delimiter[*d])
		return (safe_close(&fd), FALSE);
	if (loop_readline(new_node->delimiter[*d], &new_node->fd_heredoc[*d][1]) == -1)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror ("dup"), FALSE);
		safe_close(&fd);
		return (FALSE);		
	}
	safe_close(&new_node->fd_heredoc[*d][1]);
	new_node->redir[*f] = current->type;
	signal(SIGINT, SIG_DFL); //
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror ("dup"), FALSE);
	safe_close(&fd);
	return (TRUE);
}

int	fill_nodes_with_files(t_token **current, PARSER **new_node, t_mega_struct **mini)
{
	(*new_node)->file[(*mini)->f] = ft_strdup((*current)->value);
	if (!(*new_node)->file[(*mini)->f])
		return (FALSE);
	(*new_node)->redir[(*mini)->f++] = (*current)->type;
	return (TRUE);
}

int	fill_nodes_with_heredoc(t_token **current, PARSER **new_node, t_mega_struct **mini)
{
	if (pipe((*new_node)->fd_heredoc[(*mini)->d]) == -1)
		return (perror("pipe"), FALSE);
	(*new_node)->file[(*mini)->f] = ft_strdup("heredoc");
	if (!create_heredoc((*new_node), (*current), &(*mini)->f, &(*mini)->d))
	{
		safe_close(&(*new_node)->fd_heredoc[(*mini)->d][0]);
		reset_node_mini(*mini, &(*new_node)); // regarder si possible de mettre en une seule fonction
		free((*mini)->nodes);
		(*mini)->nodes = NULL;
		free((*new_node));
		(*new_node) = NULL;
		free_tokens(&(*mini)->tokens);
		if (g_signal == 2)
		(*mini)->exit_code = g_signal + 128;
		return (FALSE);
	}
	(*mini)->d++;
	(*mini)->f++;
	return (TRUE);		
}

int	fill_nodes_with_args(t_token **current, PARSER **new_node, t_mega_struct **mini)
{
	if ((*current)->type == ARGUMENT && (*current)->value != NULL)
	{
		while ((*current) && (*current)->value && !is_command((*current)->value))
			(*current) = (*current)->next;
		(*new_node)->command[(*mini)->cmd] = ft_strdup((*current)->value);
		if (!(*new_node)->command[(*mini)->cmd])
			return (FALSE);
		(*mini)->cmd++;
	}
	return (TRUE);
}

void	init_mini_counters(t_mega_struct *mini)
{
		mini->f = 0;
		mini->d = 0;
		mini->cmd = 0;
}

int	fill_nodes_with_(t_token *cur, PARSER *new_node, t_mega_struct *mini)
{
	if (cur->type == REDIRECT_IN || cur->type == REDIRECT_OUT || cur->type == APPEND_OUT)
		fill_nodes_with_files(&cur, &new_node, &mini);
	else if (cur->type == HEREDOC && cur->value != NULL)
	{
		if (!fill_nodes_with_heredoc(&cur, &new_node, &mini))
			return (FALSE);
	}
	else if (cur->type == ARGUMENT && cur->value != NULL)
		fill_nodes_with_args(&cur, &new_node, &mini);
	return (TRUE);
}

int	create_nodes(t_mega_struct *mini)
{
	t_token	*cur;
	PARSER		*new_node;

	cur = mini->tokens;
	while (cur)
	{
		init_mini_counters(mini);
		new_node = alloc_new_node(cur, mini->chained_env, mini->exit_code);
		if (!new_node)
			return (FALSE);
		while (cur && cur->type != PIPEX)
		{
			if (!fill_nodes_with_(cur, new_node, mini))
				return (FALSE);
			cur = cur->next;			
		}
		(add_null_to_tab(new_node, mini), add_new_node(&mini->nodes, new_node));
		if (cur && cur->type == PIPEX)
			cur = cur->next;
	}
	return (TRUE);
}

// int	create_nodes(t_mega_struct *mini)
// {
// 	t_token	*cur;
// 	PARSER		*new_node;

// 	cur = mini->tokens;
// 	while (cur)
// 	{
// 		init_mini_counters(mini);
// 		new_node = alloc_new_node(cur, mini->chained_env, mini->exit_code);
// 		if (!new_node)
// 			return (FALSE);
// 		while (cur && cur->type != PIPEX)
// 		{
// 			if (cur->type == REDIRECT_IN || cur->type == REDIRECT_OUT || cur->type == APPEND_OUT)
// 				fill_nodes_with_files(&cur, &new_node, &mini);
// 			else if (cur->type == HEREDOC && cur->value != NULL)
// 			{
// 				if (!fill_nodes_with_heredoc(&cur, &new_node, &mini))
// 					return (FALSE);
// 			}
// 			else if (cur->type == ARGUMENT && cur->value != NULL)
// 				fill_nodes_with_args(&cur, &new_node, &mini);
// 			cur = cur->next;			
// 		}
// 		(add_null_to_tab(new_node, mini), add_new_node(&mini->nodes, new_node));
// 		if (cur && cur->type == PIPEX)
// 			cur = cur->next;
// 	}
// 	return (TRUE);
// }

// int	create_nodes(t_mega_struct *mini)
// {
// 	t_token	*current;
// 	PARSER		*new_node;
// 	int		cmd;
// 	int		d;
// 	int		f;

// 	current = mini->tokens;
// 	while (current)
// 	{
// 		cmd = 0;
// 		d = 0;
// 		f = 0;
// 		new_node = alloc_new_node(current, mini->chained_env, mini->exit_code);
// 		if (!new_node)
// 			return (FALSE);
// 		while (current && current->type != PIPEX)
// 		{
// 			if (current->type == REDIRECT_IN)
// 			{
// 				new_node->file[f] = ft_strdup(current->value);
// 				if (!new_node->file[f])
// 				{
// 					return (FALSE);
// 				}
// 				new_node->redir[f++] = current->type;
// 			}
// 			else if (current->type == HEREDOC && current->value != NULL)
// 			{
// 				if (pipe(new_node->fd_heredoc[d]) == -1)
// 					return (perror("pipe"), FALSE);
// 				new_node->file[f] = ft_strdup("heredoc");
// 				if (!create_heredoc(new_node, current, &f, &d))
// 				{
// 					safe_close(&new_node->fd_heredoc[d][0]);
// 					reset_node_mini(mini, &new_node); // regarder si possible de mettre en une seule fonction
// 					free(mini->nodes);
// 					mini->nodes = NULL;
// 					free(new_node);
// 					new_node = NULL;
// 					free_tokens(&mini->tokens);
// 					if (g_signal == 2)
// 						mini->exit_code = g_signal + 128;
// 					return (FALSE);
// 				}
// 				f++;
// 				d++;			
// 			}
// 			if (current->type == REDIRECT_OUT || current->type == APPEND_OUT)
// 			{
// 					new_node->file[f] = ft_strdup(current->value);
// 					new_node->redir[f++] = current->type;
// 			}
// 			else if (current->type == ARGUMENT && current->value != NULL)
// 			{
// 				while (current && current->value && !is_command(current->value))
// 					current = current->next;
// 				if (current->type == ARGUMENT && current->value != NULL)
// 					new_node->command[cmd++] = ft_strdup(current->value);
// 			}
// 			current = current->next;
// 		}
// 		(add_null_to_tab(new_node, f, d, cmd), add_new_node(&mini->nodes, new_node));
// 		if (current && current->type == PIPEX)
// 			current = current->next;
// 	}
// 	return (TRUE);
// }
