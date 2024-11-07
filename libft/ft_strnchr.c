/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:10:37 by anvander          #+#    #+#             */
/*   Updated: 2024/11/07 14:14:06 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char	*ft_strnchr(const char *s, int c, int len)
{
	int		i;
	char	car;

	i = 0;
	car = (char) c;
	while (s[i] != '\0' && i < len)
	{
		if (s[i] == car)
			return ((char *) &s[i]);
		i++;
	}
	if (car == '\0')
		return ((char *) &s[i]);
	else
		return (NULL);
}
