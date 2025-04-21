/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:40:04 by niclee            #+#    #+#             */
/*   Updated: 2025/04/21 14:21:53 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->infile = NULL;
	cmd->limiter = NULL;
	cmd->outfile = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_argument(t_cmd *cmd, char *arg)
{
	int		i;
	char	**new_argv;

	if (!cmd || !arg)
		return ;
	i = 0;
	while (cmd->argv && cmd->argv[i])
		i++;
	new_argv = malloc(sizeof(char *) * (i + 2));
	if (!new_argv)
		return ;
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup(arg);
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

void add_cmd(t_cmd **cmds, t_cmd *new_cmd)
{
	t_cmd	*tmp;
	if (!cmds || !new_cmd)
		return ;
	if (!cmds)
		*cmds = new_cmd;
	else
	{
		tmp = *cmds;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = new_cmd;
	}
}
