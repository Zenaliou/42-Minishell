/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:24:45 by niclee            #+#    #+#             */
/*   Updated: 2025/06/30 15:51:19 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_redirection(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

t_token	*handle_word_token(t_cmd **cmd, t_token *token)
{
	if (!(*cmd))
		*cmd = new_cmd();
	if (*cmd)
		add_argument(*cmd, token->value);
	return (token->next);
}

t_cmd	*handle_pipe_syntax_error(t_cmd *cmd)
{
	ft_putstr_fd("Syntax error: unexpected '|'\n", STDERR_FILENO);
	if (cmd)
		free_cmds(cmd);
	return (NULL);
}

t_cmd	*parse_simple_command(t_token **tokens)
{
	t_cmd		*cmd;
	t_token		*current;

	cmd = NULL;
	current = *tokens;
	while (current && current->type != AND && current->type != OR
		&& current->type != PIPE)
	{
		if (current->type == WORD)
			current = handle_word_token(&cmd, current);
		else if (is_redirection(current->type))
			current = handle_redirection(&cmd, current);
		else if (current->type == LPAREN)
			current = handle_subshell(&cmd, current);
		else if (current->type == RPAREN)
			break ;
		else
			current = current->next;
	}
	*tokens = current;
	return (cmd);
}
