/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:53:14 by anvander          #+#    #+#             */
/*   Updated: 2024/10/17 15:15:10 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

// void	ft_init_struct(t_pipex *p, int ac, t_token *token, char **envp)
// {
// 	p->ac = ac;
// 	p->token->value = token;
// 	p->envp = envp;
// 	p->nb_cmd = ac - 3;
// 	if (ft_strncmp(token->type, "here_doc", 9) == 0)
// 		p->nb_cmd--;
// 	p->i = 0;
// 	p->prev_fd = -1;
// 	p->pid = 0;
// 	p->last_pid = 0;
// }

int	is_str(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!isalpha(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	list_size(t_token *list)
{
	t_token	*temp;
	int		size;

	temp = list;
	size = 0;
	while (temp != NULL)
	{
		size++;
		temp = temp->next;
	}
	return (size);
}
