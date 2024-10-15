/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 17:43:11 by cmaubert          #+#    #+#             */
/*   Updated: 2024/05/29 12:13:36 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (little[0] == '\0')
		return ((char *) big);
	while (big[i] && i < len)
	{
		while (big[i + j] == little[j]
			&& big[i] && i + j < len)
		{
			j++;
			if (little[j] == '\0')
				return ((char *) big + i);
		}
		j = 0;
		i++;
	}
	return (NULL);
}
