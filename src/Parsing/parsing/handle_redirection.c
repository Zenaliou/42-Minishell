/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:26:57 by niclee            #+#    #+#             */
/*   Updated: 2025/08/21 20:40:45 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*redir_missing_file_error(t_cmd *cmd)
{
	ft_putstr_fd("bash: syntax error near unexpected token `newline'\n",
		STDERR_FILENO);
	cmd->err = 2;
	return (NULL);
}

void	handle_redir_in(t_cmd *cmd, t_token *file_token)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(file_token->value);
}

void	handle_redir_out(t_cmd *cmd, t_token *file_token, int append)
{
	int	fd;

	fd = -1;
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup(file_token->value);
	cmd->append = append;
	if (cmd->append == 1)
		fd = open(cmd->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (cmd->append == 0)
		fd = open(cmd->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd >= 0)
		close(fd);
}

void	handle_heredoc(t_cmd *cmd, t_token *limiter_token, t_stock *stock)
{
	int	new_fd;

	new_fd = -1;
	stock->curr_cmd = cmd;
	stock->curr_token = limiter_token;
	new_fd = heredoc(stock);
	if (cmd->limiter)
		free(cmd->limiter);
	if (cmd->hd_fd > 0)
		close(cmd->hd_fd);
	cmd->heredoc = 1;
	cmd->hd_fd = new_fd;
	cmd->limiter = ft_strdup(limiter_token->value);
}

t_token	*handle_redirection(t_cmd **cmd, t_token *token, t_stock *stock)
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
		handle_heredoc(*cmd, token->next, stock);
	return (token->next->next);
}
