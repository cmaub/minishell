/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try_malloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tharchen < tharchen@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 23:16:14 by tharchen          #+#    #+#             */
/*   Updated: 2024/11/13 23:32:03 by tharchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void* try_malloc(size_t size) {
    void* p;

    p = malloc(size);
    if (p == NULL) {
        write(1, "fatal: out of memory\n", 21);
        exit(-1);
    }
    ft_bzero(p, size);
    return p;
}
