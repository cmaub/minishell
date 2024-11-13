/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:09:30 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/13 15:44:58 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

long long ft_strtoll(const char *str)
{
    long long	res;
    int 	sign;
    int	i;
    
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
        {
            res = LLONG_MAX;
            return (res);
        }
        else if (sign == -1 && (-res < (LLONG_MIN + (str[i] - '0')) / 10))
        {
            // res = LLONG_MIN;
            res = -9223372036854775807;
            return (res);
        }
        res = res * 10 + (str[i]- '0');
        i++;
    }
    return (sign * res);
}

// int main()
// {
//     printf("long long = %lld\n", ft_strtoll("9223372036854775808000"));
//     return (0);
// }