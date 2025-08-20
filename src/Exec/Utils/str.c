/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:18:00 by gule-bat          #+#    #+#             */
/*   Updated: 2025/07/31 18:18:00 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_str_n_join(char *str, char *n, char *join)
{
	char	*res;
	int		i;
	int		j;
	int		size;

	if (!str || !n || !join)
		return (NULL);
	size = ft_strlen(str) + ft_strlen(n) + ft_strlen(join);
	res = malloc((size + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	ft_bzero(res, size + 1);
	while (str[i])
		res[j++] = str[i++];
	i = 0;
	while (n[i])
		res[j++] = n[i++];
	i = 0;
	while (join[i])
		res[j++] = join[i++];
	res[j + 1] = '\0';
	return (res);
}

char	*pathchecker(char *cmd, t_env *env)
{
	char	**envsym;
	char	*copy;
	int		i;

	i = 0;
	envsym = NULL;
	envsym = getenvmini(env);
	if (!envsym)
		return (NULL);
	while (envsym[i] != NULL)
	{
		// printf("copy path %s\n", envsym[i]); DEBUG
		copy = ft_str_n_join(envsym[i], "/", cmd);
		if (!copy)
			return (freetab(envsym), NULL);
		if (access(copy, F_OK | X_OK) == 0)
		{
			break ;
		}
		else if (envsym[i + 1] != NULL)
			free(copy);
		i++;
	}
	freetab(envsym);
	return (copy);
}

// printf("l97 exec_handler\t pathchecker return (copy): %s\n", copy);

char	*pathing(t_cmd *cmds, t_env *env)
{
	char	*path;

	path = NULL;
	// if (!cmds->argv[0])
		// return (NULL);
	if (!cmds->argv)
		return (NULL);
	if (access(cmds->argv[0], F_OK | X_OK) != 0)
	{
		path = pathchecker(cmds->argv[0], env);
		if (!path)
			path = ft_strdup(cmds->argv[0]);
			// path = cmds->argv[0];
	}
	else
		// path = cmds->argv[0];
		path = ft_strdup(cmds->argv[0]);
	return (path);
}

int	checkerror(t_cmd *cmds)
{
	t_cmd *tmp;

	if (!cmds)
		return (1);
	tmp = NULL;	
	tmp = cmds;
	while (tmp)
	{
		if (tmp->err != 0)
			return (tmp->err);
		tmp = tmp->next;
	}
	return (0);
}

t_shell	shell_init(t_cmd *cmds, char **env, t_env **envi)
{
	t_shell		shell;

	ft_bzero((void *)&shell, sizeof(shell));
	shell.cmd = cmds;
	shell.head = cmds;
	shell.env = (*envi);
	shell.envtab = NULL;
	shell.in = NULL;
	shell.out = NULL;
	if (!shell.envtab)
		shell.envtab = NULL;
	shell.line = NULL;
	(void)env;
	return (shell);
}
