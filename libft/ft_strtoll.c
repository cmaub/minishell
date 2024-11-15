/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:09:30 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/15 11:58:39 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_strtoll(const char *str)
{
	long long		res;
	int				sign;
	int				i;

	res = 0;
	sign = 1;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		if (sign == 1 && (res > (LLONG_MAX - (str[i] - '0')) / 10))
			return (LLONG_MAX);
		else if (sign == -1 && (-res < (LLONG_MIN + (str[i] - '0')) / 10))
			return (-9223372036854775807);
		res = res * 10 + (str[i++] - '0');
	}
	return (sign * res);
}

// int main()
// {
//     printf("long long = %lld\n", ft_strtoll("-9223372036854775807"));
//     return (0);
// }