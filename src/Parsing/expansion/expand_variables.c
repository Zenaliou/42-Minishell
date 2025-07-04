/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:56:13 by niclee            #+#    #+#             */
/*   Updated: 2025/07/04 15:40:06 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expand_variables(t_token *tokens, char **env)
{
	t_token	*current;
	char	*expanded_value;

	current = tokens;
	while (current)
	{
		if (current->type == WORD && current->quote_state != SINGLE_QUOTE)
		{
			expanded_value = expand_all_vars(current->value, env);
			printf("CECI EST UN TEST DE TURFU : %i\n", SINGLE_QUOTE);
			printf("CECI EST UN TEST DE FUTUR : %i\n", DOUBLE_QUOTE);
			printf("CECI EST UN TEST DE PASSE : %i\n", current->quote_state);
			if (expanded_value)
			{
				free(current->value);
				current->value = expanded_value;
			}
		}
		current = current->next;
	}
}
