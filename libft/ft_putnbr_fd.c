/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maubert <maubert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:59:33 by cmaubert          #+#    #+#             */
/*   Updated: 2024/05/25 16:00:03 by maubert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	nbr;

	if (n < 0)
	{
		if (n == -2147483648)
			write (fd, "-2147483648", 11);
		else
		{
			write(fd, "-", 1);
			n = -n;
		}
	}
	if (n >= 0 && n <= 9)
	{
		nbr = n + 48;
		write(fd, &nbr, 1);
	}
	if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
}
