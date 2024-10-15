/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:47:04 by cmaubert          #+#    #+#             */
/*   Updated: 2024/05/29 10:19:10 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	car;

	i = 0;
	car = (char) c;
	while (s[i])
		i++;
	if (s[i] == '\0' && car == '\0')
		return ((char *) &s[i]);
	while (i >= 0)
	{
		if (s[i] == car)
			return ((char *) &s[i]);
		i--;
	}
	return (NULL);
}
