/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:35:04 by cmaubert          #+#    #+#             */
/*   Updated: 2024/10/18 16:22:50 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "libft.h"
// #include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h>

// char	*ft_remain_buffer(char *static_buf)
// {
// 	char	*rest;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	while (static_buf[i] != '\n' && static_buf[i] != '\0')
// 		i++;
// 	if (static_buf[i] == '\0')
// 		return (free (static_buf), NULL);
// 	rest = ft_calloc(ft_strlen(static_buf) - i + 1, sizeof(char));
// 	if (!rest)
// 		return (free(static_buf), NULL);
// 	i++;
// 	j = 0;
// 	while (static_buf[i])
// 		rest[j++] = static_buf[i++];
// 	rest[j] = '\0';
// 	free(static_buf);
// 	return (rest);
// }

// char	*ft_extract_line(char *static_buf)
// {
// 	char	*res;
// 	int		i;

// 	i = 0;
// 	if (static_buf == 0)
// 		return (NULL);
// 	while (static_buf[i] != '\0' && static_buf[i] != '\n')
// 		i++;
// 	res = ft_calloc(i + 2, sizeof(char));
// 	if (!res)
// 		return (free(static_buf), NULL);
// 	i = 0;
// 	while (static_buf[i] != '\0' && static_buf[i] != '\n')
// 	{
// 		res[i] = static_buf[i];
// 		i++;
// 	}
// 	if (static_buf[i] == '\n')
// 		res[i++] = '\n';
// 	res[i] = '\0';
// 	return (res);
// }

// char	*ft_concat(char *static_buf, char *read_buf)
// {
// 	char	*temp;

// 	temp = ft_strjoin(static_buf, read_buf);
// 	if (!temp)
// 		return (free(static_buf), NULL);
// 	free(static_buf);
// 	return (temp);
// }

// static char	*ft_read(int fd, char *static_buf, char *read_buf)
// {
// 	int			nb_bytes;

// 	nb_bytes = 1;
// 	while (nb_bytes > 0)
// 	{
// 		nb_bytes = read(fd, read_buf, BUFFER_SIZE);
// 		if (nb_bytes < 0)
// 			return (free(read_buf), NULL);
// 		else if (nb_bytes == 0)
// 			break ;
// 		read_buf[nb_bytes] = '\0';
// 		if (static_buf == NULL)
// 		{
// 			static_buf = ft_strdup("");
// 			if (!static_buf)
// 				return (NULL);
// 		}
// 		static_buf = ft_concat(static_buf, read_buf);
// 		if (!static_buf)
// 			return (free(read_buf), NULL);
// 		if (ft_strchr(read_buf, '\n'))
// 			break ;
// 	}
// 	return (static_buf);
// }

// char	*get_next_line(int fd)
// {
// 	static char	*static_buf;
// 	t_line		buf;

// 	buf.read_buf = ft_calloc (BUFFER_SIZE + 1, sizeof(char));
// 	if (!buf.read_buf)
// 		return (NULL);
// 	if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0)
// 	{
// 		if (static_buf)
// 			free(static_buf);
// 		static_buf = NULL;
// 		return (free (buf.read_buf), NULL);
// 	}
// 	static_buf = ft_read(fd, static_buf, buf.read_buf);
// 	free(buf.read_buf);
// 	if (!static_buf)
// 		return (NULL);
// 	buf.line = ft_extract_line(static_buf);
// 	if (!buf.line || fd == -42)
// 		return (free(static_buf), NULL);
// 	static_buf = ft_remain_buffer(static_buf);
// 	if (buf.line[0] == '\0' && static_buf == NULL)
// 		return (free(buf.line), NULL);
// 	return (buf.line);
// }
