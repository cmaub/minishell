/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:44:33 by cmaubert          #+#    #+#             */
/*   Updated: 2024/06/03 15:43:26 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include <stdio.h>

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*trimmed_str;

	if (!s1 || !set)
		return (ft_strdup(""));
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1) - 1;
	while (end > start && ft_strchr(set, s1[end]))
		end--;
	if (end < start)
		return (ft_strdup(""));
	trimmed_str = (char *)malloc(sizeof(char) * ((end - start) + 2));
	if (!trimmed_str)
		return (NULL);
	ft_strlcpy(trimmed_str, s1 + start, (end - start) + 2);
	trimmed_str[end - start + 1] = '\0';
	return (trimmed_str);
}
