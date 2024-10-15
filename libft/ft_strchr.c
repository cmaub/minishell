/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:22:10 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/08 11:37:26 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	car;

	i = 0;
	car = (char) c;
	while (s[i] != '\0')
	{
		if (s[i] == car)
			return ((char *) &s[i]);
		i++;
	}
	if (car == '\0')
		return ((char *) &s[i]);
	else
		return (NULL);
}
