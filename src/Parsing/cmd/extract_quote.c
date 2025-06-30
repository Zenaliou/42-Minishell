/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_quote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:07:35 by niclee            #+#    #+#             */
/*   Updated: 2025/06/30 16:10:07 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_delimiter(char c)
{
	return (c == ' ' || c == '>' || c == '<' || c == '|' || c == '&'
		|| c == '(' || c == ')');
}

static char	*extract_quoted_content(char **input, t_quote_state *state)
{
	char			*start;
	size_t			len;
	int				quote_char;

	quote_char = **input;
	if (quote_char == '\'')
		*state = SINGLE_QUOTE;
	else
		*state = DOUBLE_QUOTE;
	(*input)++;
	start = *input;
	len = 0;
	while (**input && **input != quote_char)
	{
		(*input)++;
		len++;
	}
	if (**input == quote_char)
		(*input)++;
	*state = NO_QUOTE;
	return (ft_substr(start, 0, len));
}

static char	*extract_unquoted_word(char **input, t_quote_state *state)
{
	char	*start;
	size_t	len;

	(void)state;
	start = *input;
	len = 0;
	while (**input && !is_delimiter(**input))
	{
		(*input)++;
		len++;
	}
	return (ft_substr(start, 0, len));
}

char	*extract_quoted_word(char **input, t_quote_state *state)
{
	if (**input == '\'' || **input == '"')
		return (extract_quoted_content(input, state));
	return (extract_unquoted_word(input, state));
}
