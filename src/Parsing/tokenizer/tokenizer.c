/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:07:52 by niclee            #+#    #+#             */
/*   Updated: 2025/06/30 15:18:24 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*tokenize(char *input)
{
	t_token			*tokens;
	char			*current;
	t_quote_state	quote_state;

	tokens = NULL;
	current = input;
	quote_state = NO_QUOTE;
	while (*current)
	{
		if (quote_state == NO_QUOTE && *current == ' ')
			current++;
		else if (quote_state == NO_QUOTE && *current == '>')
			current = handle_redirect_out(current, &tokens);
		else if (quote_state == NO_QUOTE && *current == '<')
			current = handle_redirect_in(current, &tokens);
		else
			current = handle_symbol(current, &tokens, &quote_state);
	}
	if (quote_state != NO_QUOTE)
		return (handle_unclosed_quote(tokens));
	return (tokens);
}
