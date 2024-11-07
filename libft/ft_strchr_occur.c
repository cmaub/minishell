/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_occur.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:19:50 by anvander          #+#    #+#             */
/*   Updated: 2024/11/07 14:22:31 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strchr_occur(const char *s, int c)
{
	int		i;
	int		nb;
	char	car;

	i = 0;
	nb = 0;
	car = (char) c;
	while (s[i] != '\0')
	{
		if (s[i] == car)
			nb++;
		i++;
	}
	return (nb);
}
