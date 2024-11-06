/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/06 18:45:25 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// penser a gerer (ou pas)ls cat de commmandes multiples sans pipe "cat ls > file"


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
	char	*save;
	char	*temp;
	char	*new_var;
	int	i;

	i = 0;
	save = ft_strchr(str, '$') + 1;
	while (*mini_env && ft_strnstr(*mini_env, save, ft_strlen(save) + 1) == NULL)
		mini_env++;
	new_var = ft_strdup(*mini_env + ft_strlen(save) + 1);
	while (str[i] != '\0' && str[i] != '$')
		i++;
	temp = ft_calloc(i + 1, sizeof(char));
	i = 0 ;
	while (str[i] != '\0' && str[i] != '$')
	{
		temp[i] = str[i];
		i++;
	}
	temp[i] = '\0';
	str = ft_strjoin(temp, new_var);
	return (str);
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

void	calculate_size_of_tab(t_token *cur, PARSER *new_node, char **mini_env)
{
	if (cur->type == REDIRECT_IN || cur->type == HEREDOC)
	{
		if (ft_strchr(cur->next->value, '$'))
			cur->next->value = return_var_from_env(cur->next->value, mini_env);
		new_node->nb_infile++;
		if (cur->type == HEREDOC)
		{
			if (ft_strchr(cur->next->value, '$'))
				cur->next->value = return_var_from_env(cur->next->value, mini_env);
		}
		new_node->nb_heredoc++;
	}
	if (cur->type == REDIRECT_OUT || cur->type == APPEND_OUT)
	{
		if (ft_strchr(cur->next->value, '$'))
			cur->next->value = return_var_from_env(cur->next->value, mini_env);
		new_node->nb_outfile++;
	}
	else if (cur->type == ARGUMENT)
	{
		if (ft_strchr(cur->value, '$'))
			cur->value = return_var_from_env(cur->value, mini_env);
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
			if (current->type == REDIRECT_IN)
			{
				new_node->infile[i] = ft_strdup(current->next->value);
				new_node->redir_type_in[i++] = current->type;
				// i++;
			}
			else if (current->type == HEREDOC)
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
			if (current->type == REDIRECT_OUT || current->type == APPEND_OUT)
			{
				new_node->outfile[o] = ft_strdup(current->next->value);
				new_node->redir_type_out[o++] = current->type;
				// o++;
			}
			else if (current->type == ARGUMENT)
			{
				new_node->command[cmd++] = ft_strdup(current->value);
				// cmd++;
			}
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

int		main(int argc, char **argv, char **env)
{
	(void)argv;
	char		*str_input;
	LEXER		*L_input = NULL;
	t_token	**tokens = NULL;
	PARSER		**nodes = NULL;
	char		**mini_env;

	mini_env = copy_env(env);	
	if (argc >= 1)
	{
		while (1)
		{
			L_input = ft_calloc(1, sizeof(L_input)); // faire une fonction pour initialiser a part
			L_input->data = NULL;
			L_input->len = 0;
			L_input->head = 0;
			tokens = ft_calloc(1, sizeof(t_token *));
			nodes = ft_calloc(1, sizeof(PARSER *));
			str_input = readline("~$");
			if (!str_input)
				break ;
			if (ft_strnstr(str_input, "exit", ft_strlen(str_input)))
				break ; // quand on appuie sur entree OU quand quote pas fermee puis fermee
			else
				rl_on_new_line();
			if (str_input)
				add_history(str_input);
			L_input->data = str_input;
			L_input->len = ft_strlen(str_input);
			if (fill_list_of_tokens(L_input, tokens))
			{}
			else
				printf("input non valide\n");
			create_nodes(tokens, nodes, mini_env);
			dprintf(2, "taille de list %d\n", ft_size_list(nodes));
			print_nodes_list(nodes);
			handle_input(nodes, mini_env, argc);
			free(tokens);
			free(str_input);
			free(nodes);
			free(L_input);
		}
	}
	return (0);
}
