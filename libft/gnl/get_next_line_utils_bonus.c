/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:26:14 by gule-bat          #+#    #+#             */
/*   Updated: 2025/01/17 17:26:14 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>

int	n_in_str(char *str, int i)
{
	int	x;

	if (!str)
		return (0);
	x = 0;
	while (str[x] != '\0')
	{
		if (str[x] == '\n' && (i == 0 || i == 2))
		{
			if (i == 2)
			{
				return (x + 1);
			}
			return (1);
		}
		x++;
	}
	if (i == 1 || i == 2)
		return (x);
	return (0);
}
// si 1, partie avant le \n, si 2 partie apres le \n;

char	*ft_strwithorn(char *str, int x)
{
	int		i;
	int		j;
	char	*dup;

	j = 0;
	dup = NULL;
	if (x == 1)
		i = n_in_str(str, 2);
	if (x == 2)
		i = n_in_str(str, 1) - n_in_str(str, 2);
	dup = ft_calloc(i + 1, sizeof(char));
	if (!dup || i == 0 || x == 0)
		return (free(dup), NULL);
	dup[i] = '\0';
	i = 0;
	if (x == 2)
		j = n_in_str(str, 2);
	while ((x == 2 && str[j]) || (str[j] && !n_in_str(dup, 0)))
	{
		dup[i] = str[j];
		i++;
		j++;
	}
	return (dup);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*nv;

	if (!s2 || !s1)
		return (NULL);
	i = n_in_str(s1, 1);
	j = n_in_str(s2, 1);
	nv = ft_calloc((i + j) + 1, sizeof(char));
	if (!nv)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		nv[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		nv[i + j] = s2[j];
		j++;
	}
	nv[i + j] = '\0';
	return (nv);
}
