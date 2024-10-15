/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:34:56 by cmaubert          #+#    #+#             */
/*   Updated: 2024/07/29 21:47:17 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	count_nbr(long nbr)
{
	int	len;

	len = 0;
	if (nbr <= 0)
	{
		len++;
		nbr = -nbr;
	}
	while (nbr > 0)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

static void	fill_result(char *result, long nbr, int len)
{
	int	i;

	i = len - 1;
	while (nbr > 0)
	{
		result[i--] = ((nbr % 10) + '0');
		nbr /= 10;
	}
}

char	*ft_itoa(int n)
{
	char	*result;
	int		len;
	long	nbr;

	nbr = (long) n;
	len = count_nbr(nbr);
	result = (char *) malloc (sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	result[len] = '\0';
	if (nbr == 0)
		result[0] = '0';
	else
	{
		if (nbr < 0)
			nbr = -nbr;
		fill_result(result, nbr, len);
		if (n < 0)
			result[0] = '-';
	}
	return (result);
}
