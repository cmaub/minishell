/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/09 13:18:33 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_status = 0;
int g_signal = 0;

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
	g_signal = signum;
	ft_putstr_fd("\n", 2);
	 // // Réinitialise l'affichage de la ligne d'entrée
	// Indique que la ligne doit être recalculée
	rl_on_new_line();
	rl_replace_line("", 0);
	// rl_done = 1;
	// // Affiche une nouvelle ligne et le prompt
	rl_redisplay();
	// exit_status = 130;
}

void	add_new_var(t_env **mini_env, t_env *new_var)
{
	t_env	*current;

	if (!new_var)
	{
		return ;
	}
	if (!(*mini_env) || !mini_env)
	{
		*mini_env = new_var;
		// dprintf(2, "mini_env->var = %s\n", (*mini_env)->var);
		return ;
	}
	else
	{
		// dprintf(2, "new_var->var = %s\n", new_var->var);
		current = *mini_env;
		while (current->next != NULL)
			current = current->next;
		current->next = new_var;
	}
}

t_env	**copy_env_list(t_env **mini_env, char **env)
{
	t_env	*new_env;
	t_env	*new_var_pwd;
	t_env	*new_var_shlvl;
	t_env	*new_var_;
	int	i;

	// t_env *test_env = NULL;
	new_env = NULL;
	
	i = 0;
	mini_env = try_malloc(sizeof(t_env **));
	if (!mini_env)
		return (NULL);
	if (!env || !(*env))
	{
		new_var_pwd = try_malloc(sizeof(t_env));
		new_var_pwd->var = ft_strjoin("PWD=", getcwd(NULL, 0));
		add_new_var(mini_env, new_var_pwd);
		new_var_shlvl = try_malloc(sizeof(t_env));
		new_var_shlvl->var = ft_strdup("SHLVL=1");;
		add_new_var(mini_env, new_var_shlvl);		
		new_var_ = try_malloc(sizeof(t_env));
		new_var_->var = ft_strdup("_=./minishell");
		add_new_var(mini_env, new_var_);		
	}
	else
	{
		while (env[i] && env[i] != NULL)
		{
			new_env = try_malloc(sizeof(t_env));
			new_env->var = ft_strdup(env[i]);
			new_env->next = NULL;
			add_new_var(mini_env, new_env);
			i++;
		}
		
	}
	return (mini_env);
}

void	print_t_env(t_env **mini_env)
{
	t_env	*current;
	
	current = *mini_env;
	// dprintf(2, "mini_env->var = %s\n", current->var);
	while (current != NULL)
	{
		dprintf(2, "mini_env->var = %s\n", current->var);
		current = current->next;
	}
}

int		main(int argc, char **argv, char **env)
{
	(void)argv;
	char		*str_input;
	LEXER		*L_input = NULL;
	t_token		*tokens = NULL;
	PARSER		*nodes = NULL;
	t_pipex	*p = NULL;
	t_env		**chained_env;
	int			exit_code = 0;
	

	chained_env = NULL;
	chained_env = copy_env_list(chained_env, env);
	print_t_env(chained_env);
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
				//dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
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
				if (create_nodes(&tokens, &nodes, chained_env, exit_code) == 0)
				{
					dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
					
					// print_tokens_list(&tokens);
					free_tokens(&tokens);	
					if (tokens == NULL)
						//dprintf(2, "tokens est freeee (%s, %d)\n", __FILE__, __LINE__);				
					// print_tokens_list(&tokens);
					// //dprintf(2, "taille de list %d\n\n", ft_size_list(&nodes));
					// print_nodes_list(&nodes);
					p = try_malloc(sizeof(*p));
					ft_init_struct(p, chained_env, nodes);
					dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
					print_t_env(p->env_nodes);
					dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
					handle_input(&nodes, p);
					dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);

					chained_env = p->env_nodes; //faire plutot copie
					// mini_env = copy_tab(p->mini_env);
					
					
					// if (p->mini_env)
					// 	ft_free_tab(p->mini_env);
					// dprintf(2, "nodes->exit_code = %d\n", nodes->exit_code);
					if (nodes)
						exit_code = nodes->exit_code;
					if (p)
					{
						free_pipex(&p); // modifier la maniere de free p avec la struct
					}
					if (p == NULL)
					{}
						//dprintf(2, "pipex est freeee (%s, %d)\n", __FILE__, __LINE__);
					if (chained_env == NULL)
					{}
						//dprintf(2, "Le mini_env est nul, dommage\n");
				}
				else if (tokens)
				{
					free_tokens(&tokens);
				}
				if (nodes)
				{
					reset_node(&nodes);
					if (nodes == NULL)
					{}
						// printf("nodes est freeee (%s, %d)\n", __FILE__, __LINE__);
				}
				//str_input deplace
			}
			// rl_replace_line("", 0);
			// rl_redisplay();
			free(str_input);
		}
	}
	if (chained_env)
	{
		//dprintf(2, "*** ici = (%s, %d)\n", __FILE__, __LINE__);
		// ft_free_tab(mini_env);
		//free la liste chainee
	}
	return (0);
}

// check history