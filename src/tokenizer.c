/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:07:52 by niclee            #+#    #+#             */
/*   Updated: 2025/04/24 19:26:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(value);
		return (NULL);
	}
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **list, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
	if (!*list)
		*list = new;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_token_type	get_token_type(char *s)
{
	if (!s)
		return (WORD);
	if (!ft_strcmp(s, "&&"))
		return (AND);
	if (!ft_strcmp(s, "||"))
		return (OR);
	if (!ft_strcmp(s, "|"))
		return (PIPE);
	if (!ft_strcmp(s, "<"))
		return (REDIR_IN);
	if (!ft_strcmp(s, ">"))
		return (REDIR_OUT);
	if (!ft_strcmp(s, ">>"))
		return (REDIR_APPEND);
	if (!ft_strcmp(s, "<<"))
		return (HEREDOC);
	return (WORD);
}

t_token *tokenize(char *input)
{
    t_token *tokens = NULL;
    char *current = input;

    while (*current)
    {
        if (*current == ' ')
        {
            current++;
        }
        else if (*current == '>')
        {
            if (*(current + 1) == '>')
            {
                add_token(&tokens, new_token(ft_strdup(">>"), REDIR_APPEND));
                current += 2;
            }
            else
            {
                add_token(&tokens, new_token(ft_strdup(">"), REDIR_OUT));
                current++;
            }
        }
        else if (*current == '<')
        {
            if (*(current + 1) == '<')
            {
                add_token(&tokens, new_token(ft_strdup("<<"), HEREDOC));
                current += 2;
            }
            else
            {
                add_token(&tokens, new_token(ft_strdup("<"), REDIR_IN));
                current++;
            }
        }
        else if (*current == '|')
        {
            add_token(&tokens, new_token(ft_strdup("|"), PIPE));
            current++;
        }
        else
        {
            char *word = extract_word(&current);
            add_token(&tokens, new_token(word, WORD));
        }
    }
    return tokens;
}
