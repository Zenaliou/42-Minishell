/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:22:31 by niclee            #+#    #+#             */
/*   Updated: 2025/06/30 15:50:38 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*find_matching_paren(t_token *tokens)
{
	int		paren_count;
	t_token	*current;

	paren_count = 1;
	current = tokens;
	while (current && paren_count > 0)
	{
		if (current->type == LPAREN)
			paren_count++;
		else if (current->type == RPAREN)
			paren_count--;
		if (paren_count > 0)
			current = current->next;
	}
	return (current);
}

t_token	*extract_tokens_between_parens(t_token *start, t_token *end)
{
	t_token	*extracted;
	t_token	*current;
	t_token	*new_tokens;

	extracted = NULL;
	current = start;
	while (current && current != end)
	{
		new_tokens = new_token(ft_strdup(current->value), current->type);
		add_token(&extracted, new_tokens);
		current = current->next;
	}
	return (extracted);
}
