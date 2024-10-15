/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:23:10 by cmaubert          #+#    #+#             */
/*   Updated: 2024/05/30 16:45:18 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dst_size)
{
	size_t	src_len;

	src_len = ft_strlen(src);
	if (src_len + 1 < dst_size)
	{
		ft_memcpy(dst, src, src_len + 1);
	}
	else if (dst_size != 0)
	{
		ft_memcpy(dst, src, dst_size - 1);
		dst[dst_size - 1] = 0;
	}
	return (src_len);
}
