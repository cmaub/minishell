/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 12:50:33 by cmaubert          #+#    #+#             */
/*   Updated: 2024/06/05 17:49:34 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	ft_bzero(void *s, size_t len)
{
	unsigned char	*tmp;

	tmp = s;
	while (len > 0)
	{
		*(tmp) = 0;
		tmp++;
		len--;
	}
}
