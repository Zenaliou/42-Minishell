/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:31:27 by niclee            #+#    #+#             */
/*   Updated: 2025/06/30 15:52:19 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_subshell_tokens(t_token *start, t_token **closing, t_token **sub)
{
	*closing = find_matching_paren(start->next);
	if (!(*closing))
		return (0);
	*sub = extract_tokens_between_parens(start->next, *closing);
	return (1);
}

t_token	*handle_subshell_error(t_cmd *cmd, t_token *sub, char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	if (sub)
		free_tokens(sub);
	if (cmd)
		free_cmds(cmd);
	return (NULL);
}

void	assign_subshell_to_cmd(t_cmd **cmd, t_cmd *sub_cmd)
{
	if (!(*cmd))
		*cmd = sub_cmd;
	else
		(*cmd)->left = sub_cmd;
}

t_token	*handle_subshell(t_cmd **cmd, t_token *token)
{
	t_token	*closing;
	t_token	*sub_tokens;
	t_cmd	*sub_cmd;

	if (!get_subshell_tokens(token, &closing, &sub_tokens))
		return (handle_subshell_error(*cmd, NULL,
				"Syntax error: unmatched '('\n"));
	sub_cmd = parse_tokens(sub_tokens);
	if (!sub_cmd)
		return (handle_subshell_error(*cmd, sub_tokens,
				"Syntax error in subcommand\n"));
	assign_subshell_to_cmd(cmd, sub_cmd);
	free_tokens(sub_tokens);
	return (closing->next);
}
