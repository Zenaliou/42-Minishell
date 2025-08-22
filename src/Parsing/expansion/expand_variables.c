/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:56:13 by niclee            #+#    #+#             */
/*   Updated: 2025/08/22 03:40:14 by gule-bat         ###   ########.fr       */
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
			if (expanded_value)
			{
				free(current->value);
				current->value = expanded_value;
			}
		}
		current = current->next;
	}
}
