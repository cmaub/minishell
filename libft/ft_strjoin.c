/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:11:24 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/08 11:37:06 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;
	size_t	len;
	int		i;
	int		j;

	len = ft_strlen(s1) + ft_strlen(s2);
	new = (char *) malloc (sizeof(char) * (len + 1));
	if (new == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
		new[j++] = s1[i++];
	i = 0;
	while (s2[i])
		new[j++] = s2[i++];
	new[j] = 0;
	return (new);
}
