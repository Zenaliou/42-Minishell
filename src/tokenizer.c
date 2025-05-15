/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:07:52 by niclee            #+#    #+#             */
/*   Updated: 2025/05/15 23:48:44 by marvin           ###   ########.fr       */
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

/*
Parametres:
    input : chaîne de caractères entrée par l’utilisateur (ex : "ls -l | grep txt > out")
Retourne:
    Une liste chaînée de tokens représentant les mots, opérateurs et redirections
Fonctionnalité:
- Parcourt chaque caractère de la chaîne `input`.
- Ignore les espaces en dehors des quotes.
- Utilise `extract_quoted_word()` pour capturer les mots et chaînes entre guillemets.
- Détecte les opérateurs (`|`, `||`, `>`, `>>`, `<`, `<<`, `&&`, etc.).
- Gère l’état des guillemets via un `quote_state` pour respecter les strings entre `'` ou `"`.
*/

t_token *tokenize(char *input)
{
    t_token *tokens = NULL;
    char *current = input;
    t_quote_state quote_state = NO_QUOTE;

    while (*current)
    {
        if (quote_state == NO_QUOTE && *current == ' ')
        {
            current++;
        }
        else if (quote_state == NO_QUOTE && *current == '>')
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
        else if (quote_state == NO_QUOTE && *current == '<')
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
        else if (quote_state == NO_QUOTE && *current == '|')
        {
            if (*(current + 1) == '|')
            {
                add_token(&tokens, new_token(ft_strdup("||"), OR));
                current += 2;
            }
            else
            {
                add_token(&tokens, new_token(ft_strdup("|"), PIPE));
                current++;
            }
        }
        else if (quote_state == NO_QUOTE && *current == '&' && *(current + 1) == '&')
        {
            add_token(&tokens, new_token(ft_strdup("&&"), AND));
            current += 2;
        }
        else if (quote_state == NO_QUOTE && *current == '(')
        {
            add_token(&tokens, new_token(ft_strdup("("), LPAREN));
            current++;
        }
        else if (quote_state == NO_QUOTE && *current == ')')
        {
            add_token(&tokens, new_token(ft_strdup(")"), RPAREN));
            current++;
        }
        else
        {
            char *word = extract_quoted_word(&current, &quote_state);
            add_token(&tokens, new_token(word, WORD));
        }
    }
    if (quote_state != NO_QUOTE)
    {
        fprintf(stderr, "Syntax error: unclosed quote\n");
        free_tokens(tokens);
        return NULL;
    }
    return (tokens);
}
