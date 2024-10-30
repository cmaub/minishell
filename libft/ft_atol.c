/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 23:41:22 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/30 10:48:26 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

// int ft_check_atol(const char *str)
// {
// 	_Bool error = false;
// 	long nb = ft_atol(str, &error);
// 	if (error == TRUE)
// 	{
// 		return (FALSE);
// 	}
// 	return (nb);
// }

int	is_space(char c)
{
	if (c && (c == ' ' || c == '\t'
			|| c == '\n' || c == '\r'
			|| c == '\v' || c == '\f'))
		return (1);
	return (0);
}

long	ft_atol(const char *str)
{
	long	num;
	int		sign;
	int		i;

	num = 0;
	sign = 1;
	i = 0;
	while (is_space(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i++] == '-')
			sign *= -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (num > (INT_MAX / 10))
			return (LONG_MAX);
		if (num < (INT_MIN / 10) || (num == (INT_MIN / 10)
				&& (str[i] - '0') < (INT_MIN % 10)))
			return (LONG_MIN);
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	return (num * sign);
}
