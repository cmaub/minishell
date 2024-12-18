/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 19:18:11 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = -1;

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

int	loop_readline_main(t_lexer **l_input, char **str)
{
	*l_input = ft_init_t_lexer_input();
	if (!*l_input)
		return (FALSE);
	signal(SIGINT, handle_c_signal);
	signal(SIGQUIT, SIG_IGN);
	*str = readline("\001\033[36;1m\002minishell ➜ \001\033[0m\002");
	if (!*str)
	{
		ft_putstr_fd("exit\n", 1);
		rl_clear_history();
		free(*l_input);
		return (FALSE);
	}
	if (*str && *str[0])
		add_history(*str);
	(*l_input)->data = *str;
	(*l_input)->len = ft_strlen(*str);
	return (TRUE);
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
		if (!loop_readline_main(&mini->l_input, &mini->str))
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
