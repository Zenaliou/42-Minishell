/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:06:52 by niclee            #+#    #+#             */
/*   Updated: 2025/06/30 16:07:23 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*extract_word(char **input)
{
	char	*start;
	size_t	len;

	start = *input;
	len = 0;
	while (**input && **input != ' ' && **input != '>' && **input != '<'
		&& **input != '|')
	{
		(*input)++;
		len++;
	}
	return (ft_substr(start, 0, len));
}
