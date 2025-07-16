/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 20:01:05 by gule-bat          #+#    #+#             */
/*   Updated: 2025/01/15 20:01:05 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>

int		n_in_str(char *str, int i);
char	*ft_calloc(size_t nitems, size_t size);
char	*ft_strwithorn(char *str, int x);
char	*ft_stash(char *stash, char *st);
char	*read_n_stock(char *rest, int fd);
char	*get_next_line(int fd);

#endif