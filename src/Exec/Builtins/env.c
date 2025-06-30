/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:45:19 by gule-bat          #+#    #+#             */
/*   Updated: 2025/06/27 17:58:20 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_env(t_env *envi)
{
	t_env	*tmp;

	//faire l'env de base de env -i avec pwd shlvl etc...
	tmp = envi;
	while (tmp)
	{
		printf("%s\n", tmp->full);
		tmp = tmp->next;
	}
	return ;
}

int	search_env(char *line, char *baseline)
{
	int	i;
	char	*cpy;
	
	i = 0;
	cpy = ft_strdup(baseline);
	if (!cpy)
		return (-1);
	while (line[i] && line[i] != '=')
		i++;
	cpy[i] = '\0';
	printf("seqrchenv return 0 %s\n", line);
	printf("seqrchenv return 0 %s\n\n", cpy);
	if (ft_strcmp(line, cpy) != 0)
		return (free(cpy), -1);
	else
		return (free(cpy), 1);
}

void	builtin_unset(char **argv, t_env **envi)
{
	t_env	*tmp;
	t_env 	**head;
	//faire l'env de base de env -i avec pwd shlvl etc...
	tmp = NULL;
	head = envi;
	if (!head)
		return ;

	(void)argv;
	while ((*head)->next != NULL && search_env(argv[1], (*head)->next->full) == -1)
		head = &(*head)->next;
	head = &(*head)->next;
	printf("deleted node = %s\n", (*head)->next->full);
	if (*head && search_env(argv[1], (*head)->full) == 1)
	{
		tmp = *head;
		printf("deleted node = %s\n", (*head)->full);
		*head = tmp->next;
		free(tmp->full);
		free(tmp);
	}
	return ;
}
	