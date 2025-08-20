/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:42:26 by niclee            #+#    #+#             */
/*   Updated: 2025/08/19 15:21:05 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*parse_pipeline(t_token **tokens, t_stock *stock)
{
	t_cmd	*cmds;
	t_cmd	*left_cmd;
	t_cmd	*right_cmd;

	cmds = NULL;
	left_cmd = parse_simple_command(tokens, stock);
	if (!left_cmd)
		return (NULL);
	add_cmd(&cmds, left_cmd);
	stock->cmd_head = cmds;
	while (*tokens && (*tokens)->type == PIPE)
	{
		*tokens = (*tokens)->next;
		right_cmd = parse_simple_command(tokens, stock);
		if (!right_cmd)
		{
			fprintf(stderr, "Syntax error: expected command after '|'\n");
			free_cmds(cmds);
			return (NULL);
		}
		add_cmd(&cmds, right_cmd);
	}
	return (cmds);
}

// static t_cmd	*handle_logical_error(t_cmd *logical_cmd, char *msg)
// {
// 	ft_putstr_fd(msg, 2);
// 	ft_putstr_fd("\n", 2);
// 	free_cmds(logical_cmd);
// 	return (NULL);
// }

// static t_cmd	*create_logical_cmd(t_cmd *left_cmd, t_token *current)
// {
// 	t_cmd	*logical_cmd;

// 	logical_cmd = new_cmd();
// 	if (!logical_cmd)
// 	{
// 		free_cmds(left_cmd);
// 		return (NULL);
// 	}
// 	logical_cmd->left = left_cmd;
// 	if (current->type == AND)
// 		logical_cmd->append = 1;
// 	else
// 		logical_cmd->append = 2;
// 	return (logical_cmd);
// }

// static t_cmd	*parse_logical_operator(t_cmd *left_cmd, t_token **current, t_env *env)
// {
// 	t_cmd	*logical_cmd;
// 	t_cmd	*right_cmd;

// 	logical_cmd = create_logical_cmd(left_cmd, *current);
// 	if (!logical_cmd)
// 		return (NULL);
// 	*current = (*current)->next;
// 	if (!*current)
// 		return (handle_logical_error(logical_cmd,
// 				"Syntax error: missing command after logical operator"));
// 	right_cmd = parse_tokens(*current, env);
// 	if (!right_cmd)
// 		return (handle_logical_error(logical_cmd,
// 				"Syntax error: invalid right side of logical operator"));
// 	logical_cmd->right = right_cmd;
// 	return (logical_cmd);
// }

t_stock	init_stock(t_env **envi, t_token **tokens)
{
	t_stock		stock;

	ft_bzero((void *)&stock, sizeof(stock));
	stock.env = *envi;
	stock.cmd_head = NULL;
	stock.curr_cmd = NULL;
	stock.token_head = *tokens;
	stock.curr_token = *tokens;
	return (stock);
}

t_cmd	*parse_tokens(t_token *tokens, t_env *env)
{
	t_token	*current;
	t_cmd	*left_cmd;
	t_stock stock;

	if (!tokens)
		return (NULL);
	current = tokens;
	stock = init_stock(&env, &tokens);
	left_cmd = parse_pipeline(&current, &stock);
	if (!left_cmd)
		return (NULL);
	// if (current && (current->type == AND || current->type == OR))
		// return (parse_logical_operator(left_cmd, &current, env));
	return (left_cmd);
}
