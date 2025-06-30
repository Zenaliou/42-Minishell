/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:26:57 by niclee            #+#    #+#             */
/*   Updated: 2025/06/30 15:27:33 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*redir_missing_file_error(t_cmd *cmd)
{
	ft_putstr_fd("Syntax error: missing file for redirection\n", STDERR_FILENO);
	if (cmd)
		free_cmds(cmd);
	return (NULL);
}

void	handle_redir_in(t_cmd *cmd, t_token *file_token)
{
	cmd->infile = ft_strdup(file_token->value);
}

void	handle_redir_out(t_cmd *cmd, t_token *file_token, int append)
{
	cmd->outfile = ft_strdup(file_token->value);
	cmd->append = append;
}

void	handle_heredoc(t_cmd *cmd, t_token *limiter_token)
{
	cmd->heredoc = 1;
	cmd->limiter = ft_strdup(limiter_token->value);
}

t_token	*handle_redirection(t_cmd **cmd, t_token *token)
{
	if (!(*cmd))
		*cmd = new_cmd();
	if (!token->next || token->next->type != WORD)
		return (redir_missing_file_error(*cmd));
	if (token->type == REDIR_IN)
		handle_redir_in(*cmd, token->next);
	else if (token->type == REDIR_OUT)
		handle_redir_out(*cmd, token->next, 0);
	else if (token->type == REDIR_APPEND)
		handle_redir_out(*cmd, token->next, 1);
	else if (token->type == HEREDOC)
		handle_heredoc(*cmd, token->next);
	return (token->next->next);
}
