/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/16 18:32:50 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = -1;

int	parser_has_reach_end(LEXER *input)
{
	if (input->head != input->len)
		return (FALSE);
	return (TRUE);
}

LEXER	*ft_init_lexer_input(void)
{
	LEXER	*new_input;

	new_input = try_malloc(sizeof(LEXER));
	if (!new_input)
		return (NULL);
	new_input->data = NULL;
	new_input->len = 0;
	new_input->head = 0;
	return (new_input);
}

void	handle_c_signal(int signum)
{
	g_signal = signum;
	ft_putstr_fd("\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// void	handle_c_signal_wait(int signum)
// {
// 	g_signal = signum;
// 	ft_putstr_fd("\n", 2);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

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
	{
		dprintf(2, "mini_env dans free_t_env est null\n");
		return ;
	}
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

// void	print_t_env(t_env **mini_env)
// {
// 	t_env	*current;

// 	if (!mini_env || !*mini_env)
// 		dprintf(2, "mini_env est null\n");
// 	current = *mini_env;
// 	while (current != NULL)
// 	{
// 		dprintf(2, "mini_env->var = %s\n", current->var);
// 		current = current->next;
// 	}
// }

int	create_full_env(t_env **mini_env, char **env)
{
	t_env	*new_env;
	int		i;

	new_env = NULL;
	i = 0;
	while (env[i] && env[i] != NULL)
	{
		new_env = try_malloc(sizeof(t_env));
		if (!new_env)
			return (free_t_env(mini_env), FALSE);
		new_env->var = ft_strdup(env[i]);
		if (!new_env->var)
			return (free_t_env(mini_env), free_t_env(&new_env), FALSE);
		new_env->next = NULL;
		add_new_var(mini_env, new_env);
		i++;
	}
	return (TRUE);
}

int	create_minimalist_env(t_env **mini_env)
{
	t_env	*new_var_pwd;
	t_env	*new_var_shlvl;
	t_env	*new_var_;

	new_var_pwd = try_malloc(sizeof(t_env));
	if (!new_var_pwd)
		return (free_t_env(mini_env), FALSE);
	new_var_pwd->var = ft_strjoin("PWD=", getcwd(NULL, 0));
	if (!new_var_pwd->var)
		return (free_t_env(mini_env), free_t_env(&new_var_pwd), FALSE);
	add_new_var(mini_env, new_var_pwd);
	new_var_shlvl = try_malloc(sizeof(t_env));
	if (!new_var_shlvl)
		return (free_t_env(mini_env), FALSE);
	new_var_shlvl->var = ft_strdup("SHLVL=1");
	if (!new_var_shlvl->var)
		return (free_t_env(mini_env), free_t_env(&new_var_shlvl), FALSE);
	add_new_var(mini_env, new_var_shlvl);
	new_var_ = try_malloc(sizeof(t_env));
	if (!new_var_)
		return (free_t_env(mini_env), FALSE);
	new_var_->var = ft_strdup("_=./minishell");
	if (!new_var_->var)
		return (free_t_env(mini_env), free_t_env(&new_var_), FALSE);
	return (add_new_var(mini_env, new_var_), TRUE);
}

t_env	**copy_env_list(t_env **mini_env, char **env)
{
	mini_env = try_malloc(sizeof(t_env));
	if (!mini_env)
		return (NULL);
	if (!env || !(*env))
	{
		if (!create_minimalist_env(mini_env))
			return (NULL);
	}
	else
	{
		if (!create_full_env(mini_env, env))
			return (NULL);
	}
	return (mini_env);
}

int	loop_readline_main(LEXER **L_input, char **str)
{
	*L_input = ft_init_lexer_input();
	if (!*L_input)
		return (FALSE);
	signal(SIGINT, handle_c_signal);
	signal(SIGQUIT, SIG_IGN);
	*str = readline("\001\033[36;1m\002minishell ➜ \001\033[0m\002");
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
		handle_input(&mini->nodes, mini->p, mini);
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

int	main(int argc, char **argv, char **env)
{
	t_mega_struct	*mini;

	((void)argv, (void)argc);
	mini = try_malloc(sizeof(t_mega_struct));
	if (!mini)
		return (FALSE);
	init_mega_struct(mini);
	mini->chained_env = copy_env_list(mini->chained_env, env);
	if (!mini->chained_env)
		return (free(mini), FALSE);
	while (1)
	{
		if (!loop_readline_main(&mini->L_input, &mini->str))
			break ;
		if (!fill_list_of_tokens(mini, mini->str))
		{
		}
		else
		{
			if (create_nodes(mini))
				free_exec_input(mini);
			(reset_node_mini(mini, NULL), free(mini->str));
		}
	}
	return (free_t_env(mini->chained_env), free(mini), TRUE);
}
