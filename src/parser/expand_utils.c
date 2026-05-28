/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:57:58 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 15:15:04 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*isolate_expand(char *str, int index)
{
	int	i;

	i = 0;
	while (str[index + i] != '\0')
	{
		if (!ft_isalnum(str[index + i]) && str[index + i] != '_')
			break ;
		i++;
	}
	return (ft_substr(str, index, i));
}

char	*join_char(char c, char *result)
{
	char	single_char[2];
	char	*tmp;

	single_char[0] = c;
	single_char[1] = '\0';
	tmp = ft_strjoin(result, single_char);
	free(result);
	if (!tmp)
		return (NULL);
	return (tmp);
}
