/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/11 16:55:49 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	add_new_var(t_env **mini_env, t_env *new_var)
{
	if (!new_var || !mini_env)
		return ;
	ft_lstadd_env_back(mini_env, new_var);
}

void	free_t_env(t_env **mini_env)
{
	t_env	*next;
	
	if (!mini_env || !(*mini_env))
		return ;
	while (*mini_env)
	{
		next = (*mini_env)->next;
		if ((*mini_env)->var)
		{
			free((*mini_env)->var);
			(*mini_env)->var = NULL;
		}
		free((*mini_env));
		*mini_env = next;
	}
	free(mini_env);
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
	mini_env = try_malloc(sizeof(t_env *));
	if (!mini_env)
		return (NULL);
	if (!env || !(*env))
	{
		new_var_pwd = try_malloc(sizeof(t_env));
		new_var_pwd->var = ft_strjoin("PWD=", getcwd(NULL, 0));
		if (!new_var_pwd->var)
			return (free(new_var_pwd), NULL);
		add_new_var(mini_env, new_var_pwd);
		new_var_shlvl = try_malloc(sizeof(t_env));
		new_var_shlvl->var = ft_strdup("SHLVL=1");
		if (!new_var_shlvl->var)
			return (free(new_var_shlvl), free_t_env(mini_env), NULL);
		add_new_var(mini_env, new_var_shlvl);		
		new_var_ = try_malloc(sizeof(t_env));
		new_var_->var = ft_strdup("_=./minishell");
		if (!new_var_->var)
			return (free(new_var_), free_t_env(mini_env), NULL);
		add_new_var(mini_env, new_var_);		
	}
	else
	{
		while (env[i] && env[i] != NULL)
		{
			new_env = try_malloc(sizeof(t_env));
			new_env->var = ft_strdup(env[i]);
			new_env->next = NULL;
			if (!new_env->var)
				return (free(new_env), free_t_env(mini_env), NULL);
			add_new_var(mini_env, new_env);
			i++;
		}		
	}
	return (mini_env);
}

void	print_t_env(t_env **mini_env)
{
	t_env	*current;
	
	if (!mini_env || !*mini_env)
		dprintf(2, "mini_env est null\n");
	current = *mini_env;
	while (current != NULL)
	{
		dprintf(2, "mini_env->var = %s\n", current->var);
		current = current->next;
	}
}

int	loop_readline_main(LEXER **L_input, char **str)
{
	*L_input = ft_init_lexer_input();
	if (!*L_input)
	{
		write(1, "fatal: out of memory\n", 21);
		return (FALSE);
	}
	signal(SIGINT, handle_c_signal);
	signal(SIGQUIT, SIG_IGN);
	*str = readline("\001\033[36;1m\002minishell ➜ \001\033[0m\002");
	// str = readline("minishell ➜ ");
	if (!*str)
	{
		ft_putstr_fd("exit\n", 1);
		rl_clear_history();
		free(*L_input);
		return (FALSE);
	}
	if (*str && *str[0])
		add_history(*str);
	(*L_input)->data = *str;
	(*L_input)->len = ft_strlen(*str);
	return (TRUE);
}

void	free_exec_input(t_mega_struct *mini)
{
	print_nodes_list(&mini->nodes);
	free_tokens(&mini->tokens);
	mini->p = try_malloc(sizeof(t_pipex));
	if (!mini->p)
	{
	}
	else
	{
		ft_init_struct(mini->p, mini->chained_env, mini->nodes);
		handle_input(&mini->nodes, mini->p);
		if (mini->nodes)
			mini->exit_code = mini->nodes->exit_code;
		free_pipex(&mini->p);
	}
}

void	init_mega_struct(t_mega_struct *mini)
{
	mini->p = NULL;
	mini->L_input = NULL;
	mini->nodes = NULL;
	mini->tokens = NULL;
	mini->chained_env = NULL;
	mini->exit_code = 0;
	mini->str = NULL;
}

int		main(int argc, char **argv, char **env)
{
	(void)argv;
	(void)argc;
	t_mega_struct	*mini;
	// char		*str;

	mini = try_malloc(sizeof(t_mega_struct));
	if (!mini)
		return (FALSE);
	init_mega_struct(mini);
	// str = NULL;
	mini->chained_env = copy_env_list(mini->chained_env, env);
	while (1)
	{
		// dprintf(2, "mini->str = %s\n", mini->str);
		if (!loop_readline_main(&mini->L_input, &mini->str))
			break ;
		if (!fill_list_of_tokens(mini, mini->str))
		{
		}
		else
		{
			if (create_nodes(mini) == 0)
				free_exec_input(mini);
			reset_node_mini(mini);
			// reset_node(&mini->nodes);
			free(mini->str);
		}
	}
	return (free_t_env(mini->chained_env), free(mini), 0);
}

