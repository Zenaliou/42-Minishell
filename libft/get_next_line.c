/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:00:49 by gule-bat          #+#    #+#             */
/*   Updated: 2025/01/15 12:00:49 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "../includes/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>

// char	*ft_calloc(size_t nitems, size_t size)
// {
// 	void			*str;
// 	size_t			total;
// 	size_t			index;
// 	unsigned char	*st;

// 	if (size == 0 || nitems == 0)
// 		return (NULL);
// 	index = 0;
// 	str = NULL;
// 	st = NULL;
// 	total = nitems * size;
// 	str = malloc(total);
// 	if (!str || total <= 0)
// 		return (free(str), free(st), NULL);
// 	st = str;
// 	while (nitems)
// 	{
// 		st[index] = (char) '\0';
// 		index++;
// 		nitems--;
// 	}
// 	return (str);
// }

char	*ft_stash(char *stash, char *st)
{
	char	*temp;
	int		i;

	i = BUFFER_SIZE;
	if (!stash || !st)
		return (NULL);
	temp = ft_strjoin(stash, st);
	if (!temp)
		return (free(stash), NULL);
	free(stash);
	while (i--)
		st[i] = '\0';
	stash = NULL;
	return (temp);
}

char	*read_n_stock(char *rest, int fd)
{
	int		sd;
	char	*rd;

	sd = -1;
	rd = NULL;
	rd = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!rd)
		return (free(rd), NULL);
	while (sd != 0)
	{
		sd = read(fd, rd, BUFFER_SIZE);
		if (sd <= 0)
		{
			if ((sd == 0 || sd < BUFFER_SIZE) && (rd[0] || rest[0]))
				return (free(rd), rest);
			return (free(rd), free(rest), NULL);
		}
		rest = ft_stash(rest, rd);
		if (!rest)
			return (free(rd), free(rest), NULL);
		if (n_in_str(rest, 0))
			break ;
	}
	free(rd);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*rest;
	char		*line;
	char		*stock;

	line = NULL;
	stock = NULL;
	if (fd < 0 || (BUFFER_SIZE <= 0) || read(fd, NULL, 0) == -1)
		return (free(rest), rest = NULL, NULL);
	if (!rest)
		rest = ft_calloc(1, sizeof(char));
	if (!rest)
		return (free(rest), NULL);
	if (!n_in_str(rest, 0))
		rest = read_n_stock(rest, fd);
	if (!rest)
		return (free(rest), free(stock), NULL);
	line = ft_strwithorn(rest, 1);
	if (!line)
		return (free(line), free(rest), NULL);
	stock = rest;
	rest = ft_strwithorn(stock, 2);
	free(stock);
	return (line);
}
// printf("[%i] LINE %s\n", n_in_str(line, 1), line);
// printf("[%i] REST %s\n", n_in_str(rest, 1), rest);
// int	main(int argc, char *argv[])
// {
// 	char		*str;
// 	static int	i;
// 	int			j;
// 	int			fd;

// 	if (argc != 2)
// 		return (0);
// 	j = 0;
// 	str = NULL;
// 	fd = open(argv[1], O_RDONLY);
// 	if (fd < 0)
// 		return (0);
// 	while (1)
// 	{
// 		str = get_next_line(fd);
// 		if (!str)
// 			break ;
// 		i = n_in_str(str, 1);
// 		j += i;
// 		// printf("--> [%i] | RESULT %s", i++, str);
// 		printf("%s", str);
// 		free(str);
// 	}
// 	free(str);
// 	// printf("--> [%i] | TOTAL", j);
// 	return (0);
// }