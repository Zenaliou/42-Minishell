/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_etc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:12:00 by gule-bat          #+#    #+#             */
/*   Updated: 2025/08/22 04:42:44 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**list_to_tab(t_env *env)
{
	char	**tab;
	int		i;
	t_env	*tmp;
	t_env	*tmp2;

	tab = NULL;
	i = 0;
	tmp = env;
	tmp2 = env;
	while (tmp2)
	{
		i++;
		tmp2 = tmp2->next;
	}
	tab = malloc((i + 1) * sizeof(char *));
	ft_bzero(tab, (i + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (tmp)
	{
		tab[i++] = ft_strdup(tmp->full);
		tmp = tmp->next;
	}
	return (tab);
}

void	buff_handling(char *buffer, int val, int i, t_env *tmp)
{
	val = ft_atol(&buffer[i]);
	free(buffer);
	buffer = ft_itoa(val + 1);
	tmp->full = ft_strjoin("SHLVL=", buffer);
	free(buffer);
}

void	close_heredoc(t_stock *stock, int *fd)
{
	close(fd[1]);
	free_cmds(stock->curr_cmd);
	free_tokens(stock->token_head);
	free_cmds(stock->cmd_head);
	free_env(stock->env);
}
