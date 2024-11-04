/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/04 19:07:32 by anvander         ###   ########.fr       */
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

void	create_nodes(t_token **tokens, PARSER **nodes)
{
	t_token	*current;
	PARSER		*new_node;
	int		i;
	int		o;
	int		cmd;

	current = *tokens;
	while (current)
	{
			
		i = 0;
		o = 0;
		cmd = 0;
		new_node = ft_calloc(1, sizeof(PARSER));
		if (!new_node)
			return ;
		new_node->infile = ft_calloc(10, sizeof(char *));
		new_node->outfile = ft_calloc(10, sizeof(char *));
		new_node->command = ft_calloc(10, sizeof(char *));
		new_node->delimiter = ft_calloc(10, sizeof(char *));
		new_node->redir_type_in = ft_calloc(10, sizeof(int));
		new_node->redir_type_out = ft_calloc(10, sizeof(int));
		while (current && current->type != PIPEX)
		{
			if (current->type == REDIRECT_IN)
			{
				new_node->infile[i] = ft_strdup(current->next->value);
				new_node->redir_type_in[i] = current->type;
				i++;
			}
			else if (current->type == HEREDOC)
			{ 
				new_node->infile[i] = ft_strdup("heredoc");
				new_node->delimiter[i] = ft_strdup(current->next->value);
				new_node->redir_type_in[i] = current->type;
				i++;
			}
			if (current->type == REDIRECT_OUT || current->type == APPEND_OUT)
			{
				new_node->outfile[o] = ft_strdup(current->next->value);
				new_node->redir_type_out[o] = current->type;
				o++;
			}
			else if (current->type == ARGUMENT)
			{
				new_node->command[cmd] = ft_strdup(current->value);
				cmd++;
			}
			current = current->next;
		}
		new_node->infile[i] = NULL;
		new_node->delimiter[i] = NULL;
		new_node->outfile[o] = NULL;
		add_new_node(nodes, new_node);
		if (current && current->type == PIPEX)
			current = current->next;
	}

}

int		main(int argc, char **argv, char **env)
{
	(void)argv;
	(void)env;
	char		*str_input;
	LEXER		*L_input = NULL;
	t_token	**tokens = NULL;
	PARSER		**nodes = NULL;
	
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
			create_nodes(tokens, nodes);
			print_nodes_list(nodes);
			handle_input(nodes, env, argc);
			free(tokens);
			free(str_input);
			free(nodes);
			free(L_input);
		}
	}
	return (0);
}
