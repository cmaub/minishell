/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   withdraw_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 10:34:17 by anvander          #+#    #+#             */
/*   Updated: 2024/12/18 15:12:06 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*withdraw_unquoted(t_parser *node, t_mega *mini, char *str, char *res)
{
	char	*tmp;
	char	*tmp_result;
	char	*expand;

	if (is_expandable(str, mini->idx, 0))
	{
		expand = expand_var(node, mini, str, &mini->idx);
		if (!expand)
			return (NULL);
		tmp_result = ft_strjoin(res, expand);
		free(expand);
		if (!tmp_result)
			return (free(res), NULL);
		free(res);
		res = tmp_result;
	}
	else
	{
		tmp = join_char(str[mini->idx], res);
		if (!tmp)
			return (free(res), NULL);
		res = tmp;
		(mini->idx)++;
	}
	return (res);
}

char	*withdraw_double(t_parser *node, t_mega *mini, char *str, char *res)
{
	char	*tmp;
	char	*tmp_result;
	char	*expand;

	if (is_expandable(str, mini->idx, 2))
	{
		expand = expand_var(node, mini, str, &mini->idx);
		if (!expand)
			return (NULL);
		tmp_result = ft_strjoin(res, expand);
		free(expand);
		if (!tmp_result)
			return (free(res), NULL);
		free(res);
		res = tmp_result;
	}
	else
	{
		tmp = join_char(str[mini->idx], res);
		if (!tmp)
			return (free(res), NULL);
		res = tmp;
		(mini->idx)++;
	}
	return (res);
}

char	*withdraw_quotes(t_parser *node, t_mega *mini, char *str)
{
	char	*result;
	char	*tmp_result;
	char	*tmp;

	mini->idx = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[mini->idx] != '\0')
	{
		tmp = process_string(node, mini, str, &mini->idx);
		if (!tmp)
			return (free(result), free(tmp_result), NULL);
		tmp_result = ft_strjoin(result, tmp);
		free(tmp);
		if (!tmp_result)
			return (free(result), NULL);
		free(result);
		result = tmp_result;
		tmp_result = NULL;
	}
	if (!result || !*result)
		return (free(result), NULL);
	return (result);
}
