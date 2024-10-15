/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 17:00:59 by cmaubert          #+#    #+#             */
/*   Updated: 2024/05/24 17:15:35 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	int					i;
	unsigned char		*ptr_s1;
	unsigned char		*ptr_s2;

	i = 0;
	ptr_s1 = (unsigned char *) s1;
	ptr_s2 = (unsigned char *) s2;
	while (i < (int) n)
	{
		if (ptr_s1[i] != ptr_s2[i])
			return (ptr_s1[i] - ptr_s2[i]);
		i++;
	}
	return (0);
}
