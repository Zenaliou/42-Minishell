/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:09:31 by gule-bat          #+#    #+#             */
/*   Updated: 2025/07/02 14:09:31 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*parser_export(char	*str)
{
	char	*content;
	int	i;
	int	j;
	int	qt;

	qt = 0;
	i = 0;
	j = 0;
	content = NULL;
	content = ft_calloc(sizeof(char *), sizeof(str));
	if (!content)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && qt == 0)
			qt = (int)str[i];
		else if (qt && str[i] == qt)
			qt = 0;
		else
			content[j++] = str[i];
		i++;
	}
	content[j] = '\0';
	return (content);
}

char	*go_to_equal(char *str)
{
	int		i;
	char	*dup;

	i = 0;
	dup = NULL;
	dup = ft_strdup(str);
	if (!dup)
		return(NULL);
	while (dup[i] && dup[i] != '=')
		i++;
	dup[i] = '\0';
	return (dup);
}

void	export2(t_env *head, t_shell *shell, char **line, char **dup)
{
	*line = parser_export(shell->cmd->argv[1]);
	*dup = go_to_equal(head->full);
}

int	builtin_export(t_shell **shell)
{
	t_env	*head;
	char	*line;
	char	*dup;
	char	*dup2;
	
	head = (*shell)->env;
	if (!(*shell)->cmd->argv[1] || ft_strncmp((*shell)->cmd->argv[1], "_=", 1) == 0)
		return (builtin_env(*shell), 0);
	while (head && head->next)
	{
		export2(head, *shell, &line, &dup);
		dup2 = go_to_equal(line);
		if (search_env(dup, dup2) == 1 && ft_strncmp((*shell)->cmd->argv[1], "_=", 1) == 0)
		{
			free(head->full);
			head->full = /*ft_strdup(*/line/*)*/;
			return free(dup), free(dup2), 1;
		}
		else if (!head->next->next)
			return (nodeend(&((*shell)->env), line), free(dup), free(line), free(dup2), 1);
		head = head->next;
		free(line);
		free(dup);
		free(dup2);
	}
	return (0);
}


// PB AVEC HEREDOC CTRL C ET V + EXPORT QUI EXPORT MM SI CA EXISTE + PIPES A FAIRE