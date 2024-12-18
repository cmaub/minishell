/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:04:35 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 14:19:04 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export makes an environment variable accessible to child processes.
// it can be used to add one or more environment variables
// if they already exist, it is possible to change their value
// if export without arg, env variables are displayed in ascii order
// check the syntax of variable names + possible values
// manage inverted commas and ‘export’ written before

int	check_value_and_add(t_env **env_n, char *cmd, int index)
{
	t_env	*temp;
	int		j;

	temp = *env_n;
	j = -1;
	if (index >= 0)
	{
		while (temp && j++ < index)
			temp = temp->next;
		if (!temp)
			return (FALSE);
		free(temp->var);
		temp->var = ft_strdup(cmd);
		if (!temp->var)
			return (FALSE);
	}
	else
		return (create_new_var(env_n, cmd));
	return (TRUE);
}

void	handle_value(char *cmd, t_parser *current, t_env **env_n)
{
	int		index;
	char	*equal;
	char	*name;

	equal = ft_strchr(cmd, '=');
	*equal = '\0';
	name = cmd;
	if (!check_name(name))
		return (print_error_msg(name, current, "': not a valid identifier"));
	index = env_var_exists(env_n, name);
	*equal = '=';
	if (!check_value_and_add(env_n, cmd, index))
		return ;
}

t_env	**handle_var_wo_value(char *cmd, t_parser *current, t_env **env_n)
{
	int		index;
	int		j;
	t_env	*new_var;

	j = 0;
	new_var = NULL;
	if (!check_name(cmd))
	{
		print_error_msg(cmd, current, "': not a valid identifier");
		return (env_n);
	}
	index = env_var_exists(env_n, cmd);
	if (index < 0)
		create_new_var(env_n, cmd);
	return (env_n);
}

t_env	**ft_export(t_parser *current, t_env **env_n)
{
	int	i;

	i = 1;
	if (!current->command[1])
		return (copy_and_sort_env(env_n), env_n);
	while (current->command[i])
	{
		if (current->command[i] && current->command[i][0] == '-')
		{
			print_error_msg(current->command[i], current, ": invalid option");
			current->exit_code = 1;
			if (i == 1)
			{
				current->exit_code = 2;
				return (env_n);
			}				
		}
		if (ft_strchr(current->command[i], '=') != NULL)
			handle_value(current->command[i], current, env_n);
		else
			handle_var_wo_value(current->command[i], current, env_n);
		i++;
	}
	return (env_n);
}
