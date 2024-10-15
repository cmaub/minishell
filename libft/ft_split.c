/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:43:38 by cmaubert          #+#    #+#             */
/*   Updated: 2024/09/19 13:01:36 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include <stdio.h>

static int	count_world(char *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (*str && *str == c)
			str++;
		if (*str)
			count++;
		while (*str && *str != c)
			str++;
	}
	return (count);
}

static size_t	word_i(char const *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

static void	free_split(char **arrays)
{
	int	i;

	i = 0;
	while (arrays[i])
	{
		free(arrays[i]);
		i++;
	}
	free(arrays);
}

char	**ft_split(char const *s, char c)
{
	char	**arrays;
	int		i;
	int		j;

	if (!s || count_world((char *) s, c) == 0)
		return (NULL);
	arrays = (char **)malloc((count_world((char *) s, c) + 1) * sizeof(char *));
	if (!arrays)
		return (NULL);
	i = 0;
	j = 0;
	while (s[j])
	{
		while (s[j] == c)
			j++;
		if (s[j])
		{
			arrays[i] = ft_substr(s, j, word_i(&s[j], c));
			if (!arrays[i++])
				return (free_split(arrays), NULL);
		}
		j += word_i(&s[j], c);
	}
	arrays[i] = NULL;
	return (arrays);
}
