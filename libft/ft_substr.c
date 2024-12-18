/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:44:34 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 18:44:11 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*new;
	size_t		n_len;
	size_t		i;

	if (!s)
		return (NULL);
	n_len = ft_strlen(s);
	if (start >= n_len)
		return (ft_strdup(""));
	if (len > n_len - start)
		len = n_len - start;
	new = ft_calloc (len + 1, sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new[i] = s[start + i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
