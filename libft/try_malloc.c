/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try_malloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:16:57 by anvander          #+#    #+#             */
/*   Updated: 2024/11/28 13:28:15 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*try_malloc(size_t size)
{
	void	*p;

	p = malloc(size);
	if (p == NULL)
	{
		write(1, "fatal: out of memory\n", 21);
		return (NULL);
	}
	ft_bzero(p, size);
	return (p);
}
