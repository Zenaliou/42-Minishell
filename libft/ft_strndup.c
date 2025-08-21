/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:52:19 by niclee            #+#    #+#             */
/*   Updated: 2025/08/21 17:22:51 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static	size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	i;

	i = 0;
	while (i < maxlen && s[i])
		i++;
	return (i);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;

	len = ft_strnlen(s, n);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	strncpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}
