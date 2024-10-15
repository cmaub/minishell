/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 12:36:12 by cmaubert          #+#    #+#             */
/*   Updated: 2024/05/29 22:03:25 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t len)
{
	unsigned char	*str;
	unsigned char	car;
	int				i;

	str = (unsigned char *) s;
	car = c;
	i = 0;
	while (i < (int) len)
	{
		str[i] = car;
		i++;
	}
	return (s);
}
