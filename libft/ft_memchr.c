/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:17:06 by cmaubert          #+#    #+#             */
/*   Updated: 2024/05/24 17:15:31 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr_s;
	unsigned char	char_c;
	int				i;

	ptr_s = (unsigned char *) s;
	char_c = (unsigned char) c;
	i = 0;
	while (i < (int) n)
	{
		if (ptr_s[i] == char_c)
			return ((void *) &ptr_s[i]);
		i++;
	}
	return (NULL);
}
