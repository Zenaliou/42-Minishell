/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:58:14 by niclee            #+#    #+#             */
/*   Updated: 2025/06/30 16:05:46 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	process_directory_entries(DIR *dir, t_token *current,
	t_token **new_tokens)
{
	struct dirent	*entry;

	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && ft_match(entry->d_name,
				current->value))
			add_token(new_tokens, new_token(ft_strdup(entry->d_name),
					WORD));
		entry = readdir(dir);
	}
}

static void	replace_token_with_matches(t_token *current, t_token *new_tokens)
{
	current->type = new_tokens->type;
	free(current->value);
	current->value = ft_strdup(new_tokens->value);
	current->next = new_tokens->next;
	free(new_tokens->value);
	free(new_tokens);
}

static void	process_wildcard_token(t_token *current)
{
	DIR		*dir;
	t_token	*new_tokens;

	dir = opendir(".");
	if (!dir)
		return ;
	new_tokens = NULL;
	process_directory_entries(dir, current, &new_tokens);
	closedir(dir);
	if (new_tokens)
		replace_token_with_matches(current, new_tokens);
}

void	expand_wildcards(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == WORD && ft_strchr(current->value, '*'))
			process_wildcard_token(current);
		current = current->next;
	}
}
