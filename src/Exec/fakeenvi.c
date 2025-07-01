/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fakeenvi.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:44:50 by gule-bat          #+#    #+#             */
/*   Updated: 2025/07/01 13:44:03 by gule-bat         ###   ########.fr       */
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

int nodenewhead(t_env **head, char *envline)
{
	*head = malloc(sizeof(t_env));
	if (!*head)
		return (-1);
	if (envline != NULL)
		(*head)->full = ft_strdup(envline);
	else
		(*head)->full = NULL;
	(*head)->next = NULL;
	return (0);
}

int	fakeenv(char **env, t_env **envi)
{
	// t_env	*head;
	int		i;

	i = 0;
	if (!env[0])
		return -1;
	if (nodenewhead(envi, env[i++]) == -1)
		return (-1);
	while (env[i] != NULL)
	{
		if (nodeend(envi, env[i++]) == -1)
			return (-1);
	}
	// head = *envi;
	return (1);
}
