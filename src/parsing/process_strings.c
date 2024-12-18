/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_strings.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:03:00 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 17:55:09 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_unquoted(t_parser *node, t_mega *mini, char *str, int *i)
{
	char	*result;
	char	*tmp_result;
	char	*tmp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	tmp_result = NULL;
	tmp = NULL;
	while (str[*i] != 39 && str[*i] != 34 && str[*i] != '\0')
		result = withdraw_unquoted(node, mini, str, result);
	return (result);
}

char	*process_single_quotes(char *str, int *index)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	tmp = NULL;
	(*index)++;
	while (str[*index] != 39 && str[*index] != '\0')
	{
		tmp = join_char(str[*index], result);
		if (!tmp)
			return (free(result), NULL);
		result = tmp;
		(*index)++;
	}
	if (str[*index] == 39)
		(*index)++;
	return (result);
}

char	*process_double(t_parser *node, t_mega *mini, char *str, int *i)
{
	char	*result;
	char	*tmp;
	char	*tmp_result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	tmp_result = NULL;
	tmp = NULL;
	(*i)++;
	while (str[*i] != 34 && str[*i] != '\0')
		result = withdraw_double(node, mini, str, result);
	if (str[*i] == 34)
		(*i)++;
	return (result);
}

char	*process_string(t_parser *node, t_mega *mini, char *str, int *i)
{
	if (str[*i] == 34)
		return (process_double(node, mini, str, i));
	else if (str[*i] == 39)
		return (process_single_quotes(str, i));
	else
		return (process_unquoted(node, mini, str, i));
}
