/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/07 15:31:03 by cmaubert         ###   ########.fr       */
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
// // fonction qui interprete $
// char	*expand(char *str, char **mini_env)
// {
// 	char	**dollars;
	
// 	dollars = NULL;
// 	if (ft_strchr_occur(str, '$') > 0)
// 	{
// 		dollars = ft_split(str, '$');
// 	}
// 	if (dollars != NULL)
// 	{
// 		// boucle qui gere chague dollar
// 		// je visualise pas comment faaaaaaire
// 	}
// 	else
// 	{
		
// 	}
// }

// // fonction qui trim "" ''
// char	*trim_quotes(char *str, int quote)
// {
	
// }



char	*return_var_from_env(char *str, char **mini_env)
{
	char	*save_after_dollar;
	char	*save_before_dollar;
	char	*rest_after_single_quote;
	char	*temp;
	char	*new_var;
	int	i;
	int	len;
	int	len_save;
	int	len_str;

	i = 0;
	len = 0;
	rest_after_single_quote = NULL;
	len_str = ft_strlen(str);
	save_after_dollar = ft_strchr(str, '$') + 1;
	len_save = ft_strlen(save_after_dollar);
	while (len_save >= 0)
	{
		len_str--;
		len_save--;
	}
	dprintf(2, "LINE = %d, len_str = %d\n", __LINE__, len_str);
	save_before_dollar = ft_calloc(len_str + 1, sizeof(char));
	ft_memcpy(save_before_dollar, str, len_str);
	if (ft_strchr_occur(save_before_dollar, 39) % 2 != 0)
		return (str);
	else if (ft_strchr(save_after_dollar, 39))
	{
		rest_after_single_quote = ft_strdup(ft_strchr(save_after_dollar, 39));
		len = ft_strlen(rest_after_single_quote);
		while (len >= 0 && save_after_dollar)
		{
			save_after_dollar[len_save - i] = '\0';
			i++;
			len--;
		}
		rest_after_single_quote = ft_strtrim(rest_after_single_quote, "'");
	}
	dprintf(2, "rest_after_single_quote = %s\n", rest_after_single_quote);
	save_after_dollar = ft_strjoin(save_after_dollar, "=");
	dprintf(2, "save_after_dollar = %s, Line = %d\n", save_after_dollar, __LINE__);
	while (*mini_env && ft_strnstr(*mini_env, save_after_dollar, ft_strlen(save_after_dollar)) == NULL)
		mini_env++;
	if (*mini_env == NULL)
	{
		if (rest_after_single_quote == NULL && save_before_dollar == NULL)
			return (NULL);
		else if (rest_after_single_quote != NULL || save_before_dollar != NULL)
		{
			return (ft_strjoin(save_before_dollar, rest_after_single_quote));
		}
	}
	new_var = ft_strdup(*mini_env + ft_strlen(save_after_dollar));
	i = 0;
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
	if (rest_after_single_quote != NULL)
	{
		rest_after_single_quote = ft_strtrim(rest_after_single_quote, "'");
		if (str != NULL)
			str = ft_strjoin(str, rest_after_single_quote);
	}
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
		{
			/*
			1- Le $ est entoure de double quotes -> c'est un char
			2- Il est precede d'un guillement mais suivi d'un char autre que guillement -> il devra expand ce qui est entre guillemets
			3- Il n'ya pas de guillemet avant mais y en a un juste apres, il expand (ca donne rien) mais n'est pas un char

			
			4- Si entoure de single quote, le dollar est un char
			5- ce qui est entre single quote suivant le dollar ne peut pas etre interprete parce qu'on a expand avant de retire les quotes
			*/
			cur->next->value = return_var_from_env(cur->next->value, mini_env);
		}
		// if (ft_strchr(cur->next->value, '39'))
			// cur->next->value = trim_quotes(cur->next->value, 39);
		// if (ft_strchr(cur->next->value, '34'))
			// cur->next->value = trim_quotes(cur->next->value, 34);
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
