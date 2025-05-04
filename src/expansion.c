/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:13:03 by niclee            #+#    #+#             */
/*   Updated: 2025/04/26 14:35:09 by niclee           ###   ########.fr       */
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
        if (current->type == WORD && current->value[0] == '$')
        {
            expanded_value = expand_env(current->value + 1, env);
            if (expanded_value)
            {
                free(current->value);
                current->value = expanded_value;
            }
        }
        current = current->next;
    }
}

char	*expand_env(char *str, char **env)
{
	int i;
	int len;

	if (!str || !env)
		return (NULL);
	len = ft_strlen(str);
	i = 0;
	while(env[i])
	{
		if (ft_strncmp(env[i], str, len) == 0 && env[i][len] == '=')
			return (ft_strdup(&env[i][len + 1]));
		i++;
	}
	return (NULL);
}

void	expand_wildcards(t_token *tokens)
{
    t_token	*current;
    DIR		*dir;
    struct dirent	*entry;
    t_token	*new_tokens;

    current = tokens;
    while (current)
    {
        if (current->type == WORD && ft_strchr(current->value, '*'))
        {
            dir = opendir(".");
            if (!dir)
                return ;
            new_tokens = NULL;
            while ((entry = readdir(dir)))
            {
                if (entry->d_name[0] != '.' && ft_match(entry->d_name, current->value))
                    add_token(&new_tokens, new_token(ft_strdup(entry->d_name), WORD));
            }
            closedir(dir);
            if (new_tokens)
            {
                current->type = new_tokens->type;
                free(current->value);
                current->value = ft_strdup(new_tokens->value);
                current->next = new_tokens->next;
                free(new_tokens->value);
                free(new_tokens);
            }
        }
        current = current->next;
    }
}

int	ft_match(const char *str, const char *pattern)
{
    if (*pattern == '\0')
        return (*str == '\0');
    if (*pattern == '*')
        return (ft_match(str, pattern + 1) || (*str && ft_match(str + 1, pattern)));
    if (*str == *pattern)
        return (ft_match(str + 1, pattern + 1));
    return (0);
}
