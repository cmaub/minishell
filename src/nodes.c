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

void	calloc_tab_of_node(PARSER *node)
{
	int	i;

	i = 0;
	if (node->nb_file > 0)
	{
		node->file = try_malloc((node->nb_file + 1) * sizeof(char *));
		node->redir = try_malloc((node->nb_file) * sizeof(int));
	}
	if (node->nb_command > 0)
		node->command = try_malloc((node->nb_command + 3) * sizeof(char *));
	if (node->nb_heredoc > 0)
	{
		node->delimiter = try_malloc((node->nb_heredoc + 1) * sizeof(char *));
		node->fd_heredoc = try_malloc((node->nb_heredoc) * sizeof(int *));
		while (i < node->nb_heredoc)
		{
			node->fd_heredoc[i] = try_malloc(2 * sizeof(int));
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

char	*expand_var(PARSER *node, t_mega_struct *mini, char *str, int *i)
{
	char	*expand;

	expand = NULL;
	if (str[(*i) + 1] && str[(*i) + 1] == '?')
	{
		expand = print_exit_code(node, i);
		if (!expand)
			return (NULL);
	}
	else
	{
		expand = print_expand(str, i, mini->chained_env);
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
			if (ft_isalpha(str[(i) + 1])
				|| str[(i) + 1] == '_'
				|| str[(i) + 1] == '?')
				return (1);
		}
		else if (flag == 0)
		{
			if ((ft_isalpha(str[(i) + 1])
					|| str[(i) + 1] == '_'
					|| str[(i) + 1] == '?'
					|| ((str[(i) + 1]) == 39
						&& (str[(i) + 2] == 39))))
				return (1);
		}
	}
	return (0);
}

char	*withdraw_0(PARSER *node, t_mega_struct *mini, char *str, char *res)
{
	char	*tmp;
	char	*tmp_result;
	char	*expand;

	if (is_expandable(str, mini->idx, 0))
	{
		expand = expand_var(node, mini, str, &mini->idx);
		if (!expand)
			return (NULL);
		tmp_result = ft_strjoin(res, expand);
		free(expand);
		if (!tmp_result)
			return (free(res), NULL);
		free(res);
		res = tmp_result;
	}
	else
	{
		tmp = join_char(str[mini->idx], res);
		if (!tmp)
			return (free(res), NULL);
		res = tmp;
		(mini->idx)++;
	}
	return (res);
}

char	*process_unquoted(PARSER *node, t_mega_struct *mini, char *str, int *i)
{
	char	*result;
	char	*tmp_result;
	char	*tmp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	tmp_result = NULL;
	tmp = NULL;
	while (str[*i] != 39 && str[*i] != 34 && str[*i] != '\0')
		result = withdraw_0(node, mini, str, result);
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
	(*index)++;
	while (str[*index] != 39 && str[*index] != '\0')
	{
		tmp = join_char(str[*index], result);
		if (!tmp)
			return (free(result), NULL);
		result = tmp;
		(*index)++;
	}
	if (str[*index] == 39)
		(*index)++;
	return (result);
}

char	*withdraw_2(PARSER *node, t_mega_struct *mini, char *str, char *res)
{
	char	*tmp;
	char	*tmp_result;
	char	*expand;

	if (is_expandable(str, mini->idx, 2))
	{
		expand = expand_var(node, mini, str, &mini->idx);
		if (!expand)
			return (NULL);
		tmp_result = ft_strjoin(res, expand);
		free(expand);
		if (!tmp_result)
			return (free(res), NULL);
		free(res);
		res = tmp_result;
	}
	else
	{
		tmp = join_char(str[mini->idx], res);
		if (!tmp)
			return (free(res), NULL);
		res = tmp;
		(mini->idx)++;
	}
	return (res);
}

char	*process_double(PARSER *node, t_mega_struct *mini, char *str, int *i)
{
	char	*result;
	char	*tmp;
	char	*tmp_result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	tmp_result = NULL;
	tmp = NULL;
	(*i)++;
	while (str[*i] != 34 && str[*i] != '\0')
		result = withdraw_2(node, mini, str, result);
	if (str[*i] == 34)
		(*i)++;
	return (result);
}

char	*process_string(PARSER *node, t_mega_struct *mini, char *str, int *i)
{
	if (str[*i] == 34)
		return (process_double(node, mini, str, i));
	else if (str[*i] == 39)
		return (process_single_quotes(str, i));
	else
		return (process_unquoted(node, mini, str, i));
}

char	*withdraw_quotes(PARSER *node, t_mega_struct *mini, char *str)
{
	char	*result;
	char	*tmp_result;
	char	*tmp;

	mini->idx = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[mini->idx] != '\0')
	{
		tmp = process_string(node, mini, str, &mini->idx);
		if (!tmp)
			return (free(result), free(tmp_result), NULL);
		tmp_result = ft_strjoin(result, tmp);
		free(tmp);
		if (!tmp_result)
			return (free(result), NULL);
		free(result);
		result = tmp_result;
		tmp_result = NULL;
	}
	if (!result || !*result)
		return (free(result), NULL);
	return (result);
}

int	update_value_in_node(t_token **cur, PARSER *node, t_mega_struct *mini)
{
	char	*tmp;

	tmp = NULL;
	tmp = ft_strdup(((*cur))->value);
	if (!tmp)
		return (FALSE);
	free((*cur)->value);
	(*cur)->value = NULL;
	(*cur)->value = withdraw_quotes(node, mini, tmp);
	free(tmp);
	if (!(*cur)->value || (*cur)->value[0] == '\0')
	{
		return (FALSE);
	}
	return (TRUE);
}

int	calculate_size_of_tab(t_token *cur, PARSER *node, t_mega_struct *mini)
{
	char	*tmp;

	tmp = NULL;
	if (cur->type == REDIRECT_IN || cur->type == HEREDOC
		|| cur->type == REDIRECT_OUT || cur->type == APPEND_OUT)
	{
		if (cur->type == HEREDOC)
			node->nb_heredoc++;
		if (cur->type)
		{
			if (!update_value_in_node(&cur, node, mini))
				return (FALSE);
		}
		node->nb_file++;
	}
	else if (cur->type == ARGUMENT)
	{
		if (!update_value_in_node(&cur, node, mini))
			return (FALSE);
		node->nb_command++;
	}
	return (TRUE);
}

PARSER	*alloc_new_node(t_token *current, t_mega_struct *mini, int exit_code)
{
	PARSER	*new_node;
	t_token	*cur;

	cur = current;
	new_node = try_malloc(sizeof(PARSER));
	if (!new_node)
		return (NULL);
	new_node->exit_code = exit_code;
	while (cur && cur->type != PIPEX)
	{
		calculate_size_of_tab(cur, new_node, mini);
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
			break ;
		}
		else
			ft_putendl_fd(input, *fd_heredoc);
		free(input);
	}
	return (0);
}

int	create_heredoc(PARSER *node, t_token *current, int *f, int *d)
{
	int	fd;

	fd = dup(STDIN_FILENO);
	if (fd == -1)
		return (perror("dup"), FALSE);
	signal(SIGINT, SIG_IGN);
	node->delimiter[*d] = ft_strdup(current->value);
	if (!node->delimiter[*d])
		return (safe_close(&fd), FALSE);
	if (loop_readline(node->delimiter[*d], &node->fd_heredoc[*d][1]) == -1)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror ("dup"), FALSE);
		safe_close(&fd);
		return (FALSE);
	}
	safe_close(&node->fd_heredoc[*d][1]);
	node->redir[*f] = current->type;
	signal(SIGINT, SIG_DFL);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror ("dup"), FALSE);
	safe_close(&fd);
	return (TRUE);
}

int	fill_nodes_with_files(t_token **cur, PARSER **node, t_mega_struct **mini)
{
	(*node)->file[(*mini)->f] = ft_strdup((*cur)->value);
	if (!(*node)->file[(*mini)->f])
		return (FALSE);
	(*node)->redir[(*mini)->f++] = (*cur)->type;
	return (TRUE);
}

int	fill_nodes_with_heredoc(t_token **cur, PARSER **node, t_mega_struct **mini)
{
	if (pipe((*node)->fd_heredoc[(*mini)->d]) == -1)
		return (perror("pipe"), FALSE);
	(*node)->file[(*mini)->f] = ft_strdup("heredoc");
	if (!create_heredoc((*node), (*cur), &(*mini)->f, &(*mini)->d))
	{
		safe_close(&(*node)->fd_heredoc[(*mini)->d][0]);
		reset_node_mini(*mini, &(*node));
		free((*mini)->nodes);
		(*mini)->nodes = NULL;
		free((*node));
		(*node) = NULL;
		free_tokens(&(*mini)->tokens);
		if (g_signal == 2)
		(*mini)->exit_code = g_signal + 128;
		return (FALSE);
	}
	(*mini)->d++;
	(*mini)->f++;
	return (TRUE);
}

int	fill_nodes_with_args(t_token **cur, PARSER **node, t_mega_struct **mini)
{
	if ((*cur)->type == ARGUMENT && (*cur)->value != NULL)
	{
		while ((*cur) && (*cur)->value && !is_command((*cur)->value))
			(*cur) = (*cur)->next;
		(*node)->command[(*mini)->cmd] = ft_strdup((*cur)->value);
		if (!(*node)->command[(*mini)->cmd])
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
	if (cur->type == REDIRECT_IN
		|| cur->type == REDIRECT_OUT
		|| cur->type == APPEND_OUT)
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
	PARSER	*new_node;

	cur = mini->tokens;
	while (cur)
	{
		init_mini_counters(mini);
		new_node = alloc_new_node(cur, mini, mini->exit_code);
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
