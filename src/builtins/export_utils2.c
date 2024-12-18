/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:11:37 by cmaubert          #+#    #+#             */
/*   Updated: 2024/12/18 17:56:47 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_var(char *sorted_env)
{
	int		j;
	char	*tmp;

	j = 0;
	ft_putstr_fd("export ", 1);
	while (*sorted_env && sorted_env[j] && sorted_env[j] != '=')
	{
		write(1, &sorted_env[j], 1);
		j++;
	}
	if (sorted_env[j] == '=')
	{
		write(1, "=\"", 2);
		tmp = ft_strnchr(sorted_env, '=', ft_strlen(sorted_env));
		if (tmp != NULL)
			ft_putstr_fd(tmp + 1, 1);
		write(1, "\"\n", 2);
	}
	else
		write(1, "\n", 1);
}

int	check_name(char *name)
{
	int	i;

	i = 0;
	if (!name || !name[0] || ft_isdigit(name[0]))
		return (FALSE);
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void	print_error_msg(char *str, t_parser *current, char *error_msg)
{
	ft_putstr_fd("export: '", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd(error_msg, 2);
	current->exit_code = 1;
}
