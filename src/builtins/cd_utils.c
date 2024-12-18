/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:05:42 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 14:06:35 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_existing_var(t_env *temp, char *dest_tmp, char *src)
{
	free(temp->var);
	temp->var = NULL;
	temp->var = ft_strjoin(dest_tmp, src);
	if (!temp->var)
		return (FALSE);
	return (TRUE);
}
