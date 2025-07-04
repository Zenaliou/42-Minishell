/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:12:52 by niclee            #+#    #+#             */
/*   Updated: 2025/07/04 15:44:07 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_symbol(char *current, t_token **tokens, t_quote_state *qs)
{
	if (*qs == NO_QUOTE && (*current == '|' || *current == '&'))
		return (handle_pipe_and_or(current, tokens));
	if (*qs == NO_QUOTE && (*current == '(' || *current == ')'))
		return (handle_parens(current, tokens));
	return (handle_word(current, tokens, qs));
}

char	*handle_pipe_and_or(char *current, t_token **tokens)
{
	if (*current == '|' && *(current + 1) == '|')
	{
		add_token(tokens, new_token(ft_strdup("||"), OR));
		return (current + 2);
	}
	if (*current == '|')
	{
		add_token(tokens, new_token(ft_strdup("|"), PIPE));
		return (current + 1);
	}
	if (*current == '&' && *(current + 1) == '&')
	{
		add_token(tokens, new_token(ft_strdup("&&"), AND));
		return (current + 2);
	}
	else if (*current == '&')
	{
		ft_putstr_fd("minishell : syntax error near unexpected token '&'\n", STDERR_FILENO);
		return (current + 1);
	}
	return (current);
}

char	*handle_parens(char *current, t_token **tokens)
{
	if (*current == '(')
	{
		add_token(tokens, new_token(ft_strdup("("), LPAREN));
		return (current + 1);
	}
	if (*current == ')')
	{
		add_token(tokens, new_token(ft_strdup(")"), RPAREN));
		return (current + 1);
	}
	return (current);
}

char	*handle_word(char *current, t_token **tokens, t_quote_state *qs)
{
	char			*word;
	t_quote_state	quote_type;
	t_token			*new;

	if (*current == '\'')
		quote_type = SINGLE_QUOTE;
	else if (*current == '"')
		quote_type = DOUBLE_QUOTE;
	else
		quote_type = NO_QUOTE;
	word = extract_quoted_word(&current, qs);
	new = new_token(word, WORD);
	new->quote_state = quote_type;
	add_token(tokens, new);
	return (current);
}

t_token	*handle_unclosed_quote(t_token *tokens)
{
	ft_putstr_fd("Syntax error: unclosed quote\n", STDERR_FILENO);
	free_tokens(tokens);
	return (NULL);
}
