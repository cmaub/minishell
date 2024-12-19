/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:55:43 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/19 10:22:56 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*return_var_from_env(char *str, t_env **chained_env)
{
	char	*new_var;
	char	*temp_str;
	t_env	*temp;

	temp = *chained_env;
	new_var = NULL;
	temp_str = ft_strjoin(str, "=");
	if (!temp_str)
		return (NULL);
	while (temp && ft_strnstr(temp->var, temp_str, ft_strlen(temp_str)) == NULL)
	{
			temp = temp-> next;
	}
	if (temp == NULL)
		return (free(temp_str), NULL);
	new_var = ft_strdup(temp->var - 1 + (ft_strlen(temp_str) + 1));
	if (!new_var)
		return (free(temp_str), NULL);
	free(temp_str);
	return (new_var);
}

char	*print_exit_code(t_parser *new_node, int *index)
{
	char	*expand_result;
	char	*itoa_result;

	itoa_result = ft_itoa(new_node->exit_code);
	if (!itoa_result)
		return (NULL);
	expand_result = ft_strdup(itoa_result);
	if (!expand_result)
		return (free(itoa_result), NULL);
	new_node->exit_code = 0;
	*index += 2;
	return (free(itoa_result), expand_result);
}

char	*print_expand(char *str, int *index, t_env **chained_env)
{
	char	*expand_expr;
	char	*expand_result;
	char	*empty;

	empty = ft_strdup("");
	if (!empty)
		return (NULL);
	expand_expr = isolate_expand(str, *index + 1);
	expand_result = return_var_from_env(expand_expr, chained_env);
	*index += ft_strlen(expand_expr) + 1;
	free(expand_expr);
	if (expand_result != NULL)
		return (free(empty), expand_result);
	return (empty);
}

char	*expand_var(t_parser *node, t_mega *mini, char *str, int *i)
{
	char	*expand;

	expand = NULL;
	if (str[(*i) + 1] && str[(*i) + 1] == '?')
	{
		expand = print_exit_code(node, i);
		if (!expand)
			return (NULL);
	}
	else
	{
		expand = print_expand(str, i, mini->chained_env);
		if (!expand)
			return (NULL);
	}
	return (expand);
}

int	is_expandable(char *str, int i, int flag)
{
	if (str[i] == '$')
	{
		if (flag == 2)
		{
			if (ft_isalpha(str[(i) + 1])
				|| str[(i) + 1] == '_'
				|| str[(i) + 1] == '?')
				return (1);
		}
		else if (flag == 0)
		{
			if ((ft_isalpha(str[(i) + 1])
					|| str[(i) + 1] == '_'
					|| str[(i) + 1] == '?'
					|| ((str[(i) + 1]) == 39
						&& (str[(i) + 2] == 39))))
				return (1);
		}
	}
	return (0);
}
