/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:14:25 by niclee            #+#    #+#             */
/*   Updated: 2025/06/30 15:17:30 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_redirect_out(char *current, t_token **tokens)
{
	if (*(current + 1) == '>')
	{
		add_token(tokens, new_token(ft_strdup(">>"), REDIR_APPEND));
		return (current + 2);
	}
	add_token(tokens, new_token(ft_strdup(">"), REDIR_OUT));
	return (current + 1);
}

char	*handle_redirect_in(char *current, t_token **tokens)
{
	if (*(current + 1) == '<')
	{
		add_token(tokens, new_token(ft_strdup("<<"), HEREDOC));
		return (current + 2);
	}
	add_token(tokens, new_token(ft_strdup("<"), REDIR_IN));
	return (current + 1);
}
