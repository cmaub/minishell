/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try_malloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:16:57 by anvander          #+#    #+#             */
/*   Updated: 2024/11/20 12:20:24 by anvander         ###   ########.fr       */
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
		exit(-1);
	}
	ft_bzero(p, size);
	return (p);
}
