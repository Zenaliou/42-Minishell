/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:13:03 by niclee            #+#    #+#             */
/*   Updated: 2025/04/21 18:33:37 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expand_variables(t_token *tokens, char **env) // $HOME, $USER
{
	
}

char	*expand_env(char *str, char **env) // $PATH, env
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
	
}