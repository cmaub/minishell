/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/03 17:32:17 by anvander         ###   ########.fr       */
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
	dprintf(2, "handle signal recu\n");
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

	mini_env = copy_tab(env);
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
				dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
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
				print_tokens_list(&tokens);
				ft_putendl_fd("syntax error", 2);
				exit_code = 2;
				// free_tokens(tokens);
			}
			// free_tokens(tokens);
			else
			{
				if (create_nodes(&tokens, &nodes, mini_env, exit_code) == 0)
				{
					print_tokens_list(&tokens);
					free_tokens(&tokens);	
					if (tokens == NULL)
						dprintf(2, "tokens est freeee (%s, %d)\n", __FILE__, __LINE__);				
					// print_tokens_list(&tokens);
					// dprintf(2, "taille de list %d\n\n", ft_size_list(&nodes));
					print_nodes_list(&nodes);
					p = try_malloc(sizeof(*p));
					ft_init_struct(p, mini_env, nodes);
					handle_input(&nodes, p);
					
					mini_env = copy_tab(p->mini_env);
					// if (p->mini_env)
					// 	ft_free_tab(p->mini_env);
					if (nodes)
						exit_code = nodes->exit_code;
					if (p)
						free_pipex(&p);
					if (p == NULL)
						dprintf(2, "pipex est freeee (%s, %d)\n", __FILE__, __LINE__);
					// if (mini_env == NULL)
					// 	dprintf(2, "Le mini_env est nul, dommage\n");
				}
				else if (tokens)
					free_tokens(&tokens);
				if (nodes)
				{
					reset_node(&nodes);
					// if (nodes == NULL)
					// 	printf("nodes est freeee (%s, %d)\n", __FILE__, __LINE__);

					
				}
				dprintf(0, "hello\n");

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