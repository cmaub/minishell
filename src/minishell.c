/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:57:19 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/15 11:37:41 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// char ft_readline(char **argv)
// {
// 	char	*prompt;
// 	char

// 	prompt = "~$";
// 	readline()
// }

int		main(int argc, char **argv)
{
	(void)argv;
	char	*input;
	
	if (argc >= 1)
	{
		while (1)
		{
			input = readline("~$");
			if (!input)
				break ;
			if (input)
				add_history(input);
			printf("Ligne tapee %s\n", input);
		free (input);
		}
	}
	return (0);
}