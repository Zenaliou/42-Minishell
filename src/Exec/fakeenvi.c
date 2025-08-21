/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fakeenvi.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:44:50 by gule-bat          #+#    #+#             */
/*   Updated: 2025/08/21 18:26:38 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <../includes/minishell.h>

int	nodeend(t_env **head, char	*env)
{
	t_env	*new;
	t_env	*cont;

	if (!*head)
		return (-1);
	new = NULL;
	cont = NULL;
	new = malloc(sizeof(t_env));
	if (!new)
		return (-1);
	if (env != NULL)
		new->full = ft_strdup(env);
	new->next = NULL;
	cont = *head;
	while (cont->next != NULL)
		cont = cont->next;
	cont->next = new;
	return (0);
}

int	nodenewhead(t_env **head, char *envline)
{
	t_env	*new;

	new = NULL;
	if (!envline)
		return (-1);
	new = malloc(sizeof(t_env));
	if (!new)
		return (-1);
	(new)->full = ft_strdup(envline);
	if (!(new)->full)
		return (free(new), -1);
	(new)->next = NULL;
	*head = new;
	return (0);
}

int	fakeenvempty(t_env **envi)
{
	if (nodenewhead(envi, ft_strjoin("PWD=", getcwd(NULL, 0))) == -1)
		return (-1);
	if (nodeend(envi, "SHLVL=1") == -1)
		return (-1);
	if (nodeend(envi, "_=/usr/bin/env") == -1)
		return (-1);
	if (nodeend(envi,
			"PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin")
		== -1)
		return (-1);
	return (1);
}

void	shell_level(t_env **envi)
{
	t_env	*tmp;
	int		val;
	int		i;
	char	*buffer;

	tmp = *envi;
	val = 0;
	i = 0;
	buffer = NULL;
	while (tmp)
	{
		buffer = tmp->full;
		if (search_env("SHLVL", buffer) == 1)
		{
			while (buffer[i] != '=')
				i++;
			i++;
			if (ft_isdigit(buffer[i]))
				buff_handling(buffer, val, i, tmp);
			return ;
		}
		tmp = tmp->next;
	}
	return ;
}

int	fakeenv(char **env, t_env **envi)
{
	int		i;

	i = 0;
	ft_bzero((void *)envi, sizeof(t_env));
	if (!env[0])
	{
		fakeenvempty(envi);
		shell_level(envi);
		return (1);
	}
	if (nodenewhead(envi, env[i++]) == -1)
		return (-1);
	while (env[i] != NULL)
	{
		if (nodeend(envi, env[i++]) == -1)
			return (-1);
	}
	shell_level(envi);
	return (1);
}
