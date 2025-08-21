/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/07/02 14:09:31 by gule-bat          #+#    #+#             */
/*   Updated: 2025/07/02 14:09:31 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*parser_export(char *str)
{
	char	*content;
	int		i;
	int		j;
	int		qt;

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

void	export2(t_env *head, t_shell *shell, char **line, char **dup)
{
	*line = parser_export(shell->cmd->argv[1]);
	*dup = go_to_equal(head->full);
}

static int	has_equal_sign(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

static int	update_env_if_exists(t_env *head, char *line, char *dup2,
		int has_equal)
{
	char	*dup;

	while (head)
	{
		dup = go_to_equal(head->full);
		if (ft_strcmp(dup, dup2) == 0)
		{
			if (has_equal)
			{
				free(head->full);
				head->full = line;
			}
			free(dup);
			free(dup2);
			return (1);
		}
		free(dup);
		head = head->next;
	}
	return (0);
}

int	builtin_export(t_shell **shell)
{
	t_env	*head;
	char	*line;
	char	*dup2;
	int		has_equal;

	head = (*shell)->env;
	if (!(*shell)->cmd->argv[1] || ft_strncmp((*shell)->cmd->argv[1], "_=",
			1) == 0)
		return (builtin_env(*shell), 0);
	line = parser_export((*shell)->cmd->argv[1]);
	dup2 = go_to_equal(line);
	has_equal = has_equal_sign(line);
	if (update_env_if_exists(head, line, dup2, has_equal))
		return (1);
	if (has_equal)
		nodeend(&((*shell)->env), line);
	else
		free(line);
	free(dup2);
	return (1);
}

// PB AVEC HEREDOC CTRL C ET V + PIPES A FAIRE