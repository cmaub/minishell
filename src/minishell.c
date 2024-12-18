/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 18:41:55 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = -1;

int	parser_has_reach_end(t_lexer *input)
{
	if (input->head != input->len)
		return (FALSE);
	return (TRUE);
}

t_lexer	*ft_init_t_lexer_input(void)
{
	t_lexer	*new_input;

	new_input = try_malloc(sizeof(t_lexer));
	if (!new_input)
		return (NULL);
	new_input->data = NULL;
	new_input->len = 0;
	new_input->head = 0;
	return (new_input);
}

void	add_new_var(t_env **mini_env, t_env *new_var)
{
	if (!new_var || !mini_env)
		return ;
	ft_lstadd_env_back(mini_env, new_var);
}



// A isoler dans un fichier a part et pas l'appeler
void	print_t_env(t_env **mini_env)
{
	t_env	*current;

	if (!mini_env || !*mini_env)
	{
	}
	current = *mini_env;
	while (current != NULL)
		current = current->next;
}

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
			return (free_env(mini_env), FALSE);
		new_env->var = ft_strdup(env[i]);
		if (!new_env->var)
			return (free_env(mini_env), free_env(&new_env), FALSE);
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
		return (free_env(mini_env), FALSE);
	new_var_pwd->var = ft_strjoin("PWD=", getcwd(NULL, 0));
	if (!new_var_pwd->var)
		return (free_env(mini_env), free_env(&new_var_pwd), FALSE);
	add_new_var(mini_env, new_var_pwd);
	new_var_shlvl = try_malloc(sizeof(t_env));
	if (!new_var_shlvl)
		return (free_env(mini_env), FALSE);
	new_var_shlvl->var = ft_strdup("SHLVL=1");
	if (!new_var_shlvl->var)
		return (free_env(mini_env), free_env(&new_var_shlvl), FALSE);
	add_new_var(mini_env, new_var_shlvl);
	new_var_ = try_malloc(sizeof(t_env));
	if (!new_var_)
		return (free_env(mini_env), FALSE);
	new_var_->var = ft_strdup("_=./minishell");
	if (!new_var_->var)
		return (free_env(mini_env), free_env(&new_var_), FALSE);
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

int	loop_readline_main(t_lexer **L_input, char **str)
{
	*L_input = ft_init_t_lexer_input();
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

void	init_mega(t_mega *mini)
{
	mini->p = NULL;
	mini->L_input = NULL;
	mini->nodes = NULL;
	mini->begin = NULL;
	mini->tokens = NULL;
	mini->chained_env = NULL;
	mini->exit_code = 0;
	mini->str = NULL;
	mini->f = 0;
	mini->d = 0;
	mini->cmd = 0;
	mini->idx = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_mega	*mini;

	((void)argv, (void)argc);
	mini = try_malloc(sizeof(t_mega));
	if (!mini)
		return (FALSE);
	init_mega(mini);
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
			(rst_nde_mini(mini, NULL), free(mini->str));
		}
	}
	return (free_env(mini->chained_env), free(mini), TRUE);
}
void    init_pipex(t_pipex *p, t_env **chained_env, t_parser *nodes)
{
    p->env_n = chained_env;
    p->nb_cmd = ft_size_list(&nodes);
    p->i = 0;
    p->d = 0;
    p->prev_fd = -1;
    p->pid = 0;
    p->last_pid = 0;
    p->exit = 0;
    p->flag = 0;
    p->pipefd[0] = -1;
    p->pipefd[1] = -1;
}
