/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:35:12 by niclee            #+#    #+#             */
/*   Updated: 2025/08/21 15:03:46 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		argv[i] = NULL;
		i++;
	}
	free(argv);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		free_argv(tmp->argv);
		if (tmp->infile)
			free(tmp->infile);
		if (tmp->outfile)
			free(tmp->outfile);
		if (tmp->limiter)
			free(tmp->limiter);
		if (tmp->hd_fd >= 0)
			close(tmp->hd_fd);
		if (tmp->left)
			free_cmds(tmp->left);
		if (tmp->right)
			free_cmds(tmp->right);
		free(tmp);
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	tmp = NULL;
	while (env)
	{
		tmp = env;
		env = env->next;
		if (tmp->full)
			free(tmp->full);
		free(tmp);
	}
}

void	freetab(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
		free(str[i++]);
	free(str);
}
