/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:07:52 by niclee            #+#    #+#             */
/*   Updated: 2025/04/17 10:25:03 by niclee           ###   ########.fr       */
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

t_token	*tokenize(char *line)
{
	t_token	*tokens;
	char	*start;
	char	quote;
	
	tokens = NULL;
	while (*line)
	{
		while (*line && (*line == ' ' || *line == '\t'))
			line++;
		start = line;
		if (*line == '\"' || *line == '\'')
		{
			quote = *line++;
			start = line;
			while (*line && *line != quote)
				line++;
			add_token(&tokens, new_token(ft_strndup(start, line - start), WORD));
			if (*line)
				line++;
		}
		else if (!ft_strncmp(line, ">>", 2) || !ft_strncmp(line, "<<", 2))
		{
			add_token(&tokens, new_token(ft_strndup(line, 2), get_token_type(line)));
			line += 2;
		}
		else if (*line == '|' || *line == '<' || *line == '>')
		{
			add_token(&tokens, new_token(ft_strndup(line, 1), get_token_type(line)));
			line++;
		}
		else
		{
			start = line;
			while (*line && *line != ' ' && *line != '\t' && *line != '|' && *line != '<' && *line != '>')
				line++;
			add_token(&tokens, new_token(ft_strndup(start, line - start), WORD));
		}
	}
	return (tokens);
}

// TESTING FUNCTION

// void print_tokens(t_token *tokens)
// {
//     while (tokens)
//     {
//         printf("Token: '%s', Type: %d\n", tokens->value, tokens->type);
//         tokens = tokens->next;
//     }
// }

// int main(void)
// {
//     char line[1024];
//     t_token *tokens;

//     printf("Enter a command line to tokenize: ");
//     if (fgets(line, sizeof(line), stdin) == NULL)
//     {
//         perror("Error reading input");
//         return (1);
//     }
//     size_t len = strlen(line);
//     if (len > 0 && line[len - 1] == '\n')
//         line[len - 1] = '\0';

//     tokens = tokenize(line);
//     if (!tokens)
//     {
//         printf("No tokens generated.\n");
//         return (1);
//     }
//     printf("Generated tokens:\n");
//     print_tokens(tokens);
//     t_token *tmp;
//     while (tokens)
//     {
//         tmp = tokens;
//         tokens = tokens->next;
//         free(tmp->value);
//         free(tmp);
//     }
//     return (0);
// }