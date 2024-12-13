/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 12:59:24 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/11 18:16:07 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_fd(char *s, int fd)
{
	if (write (fd, s, ft_strlen(s)) == (ssize_t)ft_strlen(s))
		return (0);
	write(2, "Write: try again\n", 17);
	return (-1);
}
