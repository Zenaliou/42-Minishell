/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:54:37 by niclee            #+#    #+#             */
/*   Updated: 2025/04/11 14:21:47 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char *ft_strndup(const char *s, size_t n)
{
    char *dup;
    size_t len = strnlen(s, n);

    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    strncpy(dup, s, len);
    dup[len] = '\0';
    return (dup);
}
