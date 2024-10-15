/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 09:47:28 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/08 11:36:52 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include <stdint.h>

void	*ft_calloc(size_t count, size_t size)
{
	char	*ptr;
	size_t	i;

	i = 0;
	if (count != 0 && (count * size) / count != size)
		return (NULL);
	ptr = (void *)malloc (count * size);
	if (!ptr)
		return (NULL);
	while (i < (count * size))
	{
		ptr[i] = 0;
		i++;
	}
	return (ptr);
}
