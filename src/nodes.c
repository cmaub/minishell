/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 10:34:17 by anvander          #+#    #+#             */
/*   Updated: 2024/12/04 16:38:15 by cmaubert         ###   ########.fr       */
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

	tmp = (*nodes);
	if (!(*nodes))
		return ;
	while (index <= ft_size_list(nodes))
	{
		f = 0;
		h = 0;
		d = 0;
		while (f < 30 && tmp->file && tmp->file[f] != NULL)
		{
			printf("tmp->file[%d] = %s, type = %d\n", f, tmp->file[f], tmp->redir_type[f]);
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
	{
		return ;
	}
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
		new_node->redir_type = try_malloc((new_node->nb_file) * sizeof(int));
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

char	*return_var_from_env(char *str, char **mini_env)
{
	char	*new_var;

	new_var = NULL;
	str = ft_strjoin(str, "=");
	if (!str)
		return (NULL);
	while (*mini_env && ft_strnstr(*mini_env, str, ft_strlen(str)) == NULL)
		mini_env++;
	if (*mini_env == NULL)
		return (NULL);
	new_var = ft_strdup(*mini_env - 1 + (ft_strlen(str) + 1));
	if (!new_var)
		return (free(str), NULL);
	free(str);
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

char	*join_char(char c, char *tmp)
{
	char	single_char[2];
	char	*result;

	single_char[0] = c;
	single_char[1] = '\0';
	result = ft_strjoin(tmp, single_char);
	if (!result)
		return (NULL);
	return (result);
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
			{
				tmp = print_exit_code(result, new_node, index);
				if (!tmp)
					return (free(result), NULL);
			}
			else
			{
				tmp = print_expand(str, &(*index), mini_env);
				if (!tmp)
					return (free(result), NULL); //verifier retour ici
			}
			result = ft_strjoin(result, tmp);
			if (!result)
				return(free(result), free(tmp), NULL);
			free(tmp);
		}
		else
		{
			
			tmp = join_char(str[*index], result);
			if (!tmp)
				return (free(result), NULL);
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
		if (!tmp)
			return (free(result), NULL);
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
			if (!result)
				return (free(tmp), NULL);
			free(tmp);
		}
		else
		{
			tmp = join_char(str[*index], result);
			if (!tmp)
				return (free(result), NULL);
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
		if (!result)
			return (free(tmp), NULL);
		free(tmp);
	}
	return (result);
}

void	calculate_size_of_tab(t_token *cur, PARSER *new_node, char **mini_env)
{
	char	*tmp;

	tmp = NULL;
	if (cur->type == REDIRECT_IN || cur->type == HEREDOC 
		|| cur->type == REDIRECT_OUT || cur->type == APPEND_OUT)
	{
		if (cur->type == HEREDOC)
			new_node->nb_heredoc++;
		if (cur->type) // checker que next n'est pas un pb
		{
			tmp = ft_strdup(cur->value);
			free(cur->value);
			cur->value = withdraw_quotes(new_node, tmp, mini_env);
			free(tmp);
		}
		new_node->nb_file++;
	}
	else if (cur->type == ARGUMENT)
	{
		tmp = ft_strdup(cur->value);
		free(cur->value);
		cur->value = withdraw_quotes(new_node, tmp, mini_env);
		free(tmp);
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

void	add_null_to_tab(PARSER *new_node, int f, int d, int cmd)
{
	if (f > 0)
		new_node->file[f] = NULL;
	if (d > 0)
		new_node->delimiter[d] = NULL;
	if (cmd > 0)
		new_node->command[cmd] = NULL;
}

// void	handle_c_signal_heredoc(int sig, siginfo_t *info, void *context)
// {
// 	(void)sig;
// 	(void)info;
// 	t_pipe_fds_heredoc	*fds;
	
// 	// dprintf(2, "entree dans handle_c_signal_heredoc\n");
// 	dprintf(2, "Adresse reçue pour context : %p\n", (void *)context);
// 	if (!context)
// 		dprintf(2, "context pas valide\n");
// 	fds = (t_pipe_fds_heredoc *)context;
// 	dprintf(2, "fds dans signal C ---- fds.fd[0] = %d, fds.fd[1] = %d\n", fds->fd[0], fds->fd[1]);
// 	safe_close(&fds->fd[0]);
// 	safe_close(&fds->fd[1]);
// 	dprintf(2, "Ligne = %d fds apres close -- fds->fd[0] = %d, fds->fd[1] = %d\n", __LINE__, fds->fd[0], fds->fd[1]);

// 	// free(fds);
// 	exit_status = 130;
// 	write(1, "\n", 1);
// 	dprintf(2, "sorte de handle_c_signal_heredoc\n");
// 	exit(130);
// }//rl_done
// void	get_lines(char *delimiter, int *fd_heredoc)
// {
// 	char	*str;

// 	signal(SIGINT, handle_c_signal_heredoc);
// 	while (1)
// 	{
// 		write(1, "heredoc> ", 9);
// 		str = get_next_line(0);
// 		if (g_signal == SIGINT)
// 		{
// 			ft_putendl_fd("signal recu heredoc interrompu", 2);
// 			free(str);
// 			safe_close(fd_heredoc);
// 			return ;
// 		}
// 		if (!str)
// 		{
// 			ft_putendl_fd("warning: here-document delimited by end-of-file", 2);
// 			safe_close(fd_heredoc);
// 			return ;
// 		}
// 		if (ft_strncmp(str, delimiter, ft_strlen(delimiter)) == 0
// 			&& str[ft_strlen(delimiter)] == '\n')
// 		{
// 			free(str);
// 			get_next_line(-42);
// 			safe_close(fd_heredoc);
// 			return ;
// 		}
// 		write(*fd_heredoc, str, ft_strlen(str));
// 		free(str);
// 	}
// 	safe_close(fd_heredoc);
// }
void	handle_c_signal_heredoc(int signum)
{
	g_signal = signum;
	write(1, "\n", 1);
	// rl_on_new_line();
	rl_replace_line("", 0);
	// rl_redisplay();
	rl_done = 1;
	close(STDIN_FILENO);
}

int	loop_readline(char *delimiter, int *fd_heredoc)
{
	char	*input;

	// signal(SIGINT, SIG_DFL);
	signal(SIGINT, handle_c_signal_heredoc);
	while (g_signal == 0)
	{
		input = readline("heredoc> ");
		if (g_signal == SIGINT)
		{
			free(input);
			safe_close(fd_heredoc);
			return (-1);
		}
		if (!input)
		{
			ft_putendl_fd("warning: here-document delimited by end-of-file", 2);
			safe_close(fd_heredoc);
			return (0);
		}
		if (ft_strncmp(input, delimiter, ft_strlen(delimiter)) == 0)
		{
			safe_close(fd_heredoc);
			free(input);
			break;
		}
		else //verifier variable globale
		{
			ft_putendl_fd(input, *fd_heredoc);
		}
		free(input);
	}
	return (0);
}

int	create_heredoc(PARSER *new_node, t_token *current, int *f, int *d)
{
	int	fd;

	fd = dup(STDIN_FILENO);
	if (fd == -1)
	{
		perror("dup");
		return (0);
	}
	
	signal(SIGINT, SIG_IGN); //
	
	new_node->delimiter[*d] = ft_strdup(current->value); //ft_strdup(current->next->value)	
	loop_readline(new_node->delimiter[*d], &new_node->fd_heredoc[*d][1]);
	
	// get_lines(new_node->delimiter[*d], &new_node->fd_heredoc[*d][1]);
	safe_close(&new_node->fd_heredoc[*d][1]);
	new_node->redir_type[*f] = current->type;
	
	signal(SIGINT, SIG_DFL); //

	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror ("dup");
		return (0);
	}
	safe_close(&fd);
	if (g_signal == SIGINT)
	{
		dprintf(2, "g_signal = 2\n");
		// g_signal = 0;
		// exit_status = 130;
		// safe_close(&new_node->fd_heredoc[*d][0]);
		// safe_close(&new_node->fd_heredoc[*d][1]);
		return (0);
	}
	return (0);
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
			// free_tokens(*tokens);
			return (-1);
		}
		while (current && current->type != PIPEX)
		{
			if (current->type == REDIRECT_IN)
			{
				new_node->file[f] = ft_strdup(current->value);
				new_node->redir_type[f++] = current->type;
			}
			else if (current->type == HEREDOC && current->value != NULL)
			{
				if (pipe(new_node->fd_heredoc[d]) == -1)
				{
					perror("pipe");
					return (-1);
				}
				new_node->file[f] = ft_strdup("heredoc");
				if (create_heredoc(new_node, current, &f, &d) == -1)
				{
					// free_tokens(*tokens);
					return (-1);
				}
				f++;
				d++;
				
			}
			if (current->type == REDIRECT_OUT || current->type == APPEND_OUT)
			{
					new_node->file[f] = ft_strdup(current->value);
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
	// free_tokens(*tokens);
	return (0);
}