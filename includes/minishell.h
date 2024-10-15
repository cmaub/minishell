/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:32:21 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/15 18:38:09 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include "libft.h"

#ifndef true
# define true 1
#endif

#ifndef false
# define false 0
#endif

typedef struct s_data
{
    char          *value;
    struct s_type *type;
    struct s_data *next;    
}t_data;

typedef struct s_type
{
    char    *cmd;
    char    *option;
    char    *argument;
    char    *redirection;
    int     input;
    int     output;
}t_type;

void	ft_lstadd(t_data **list, t_data *new);
void	print_list(t_data *list);

t_data	*create_node(char *input);

#endif