/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:06:25 by cmaubert          #+#    #+#             */
/*   Updated: 2024/06/04 12:19:35 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t len)
{
	size_t					i;
	unsigned char			*d;
	const unsigned char		*s;

	if (!dest && !src)
		return (NULL);
	d = (unsigned char *) dest;
	s = (const unsigned char *) src;
	i = 0;
	while (i < len)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
