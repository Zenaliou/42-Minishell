/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:45:19 by gule-bat          #+#    #+#             */
/*   Updated: 2025/06/26 15:28:12 by gule-bat         ###   ########.fr       */
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

// int	search_env(char *line, char *baseline)
// {
// 	int	i;
// 	int j;
// 	int	k;
	
// 	j = 0;
// 	i = 0;
// 	k = 0;
// 	while (baseline[k] != '=')
// 		k++;
// 	while (line[i])
// 	{
// 		j = 0;
// 		if (line[i] == 34 || line[i] == 39)
// 			i++;
// 		if (line[i] == baseline[j])
// 		{
// 			while (line[i+j] == baseline[j])
// 			{
// 				if (j == k && baseline[j] == line[i+j])
// 					return (printf("exit positive\n"), 1);
// 				j++;
// 			}
// 			i++;
// 		}
// 		i++;
// 	}
// 	printf("seqrchenv return 0 %s\n", &line[i]);
// 	return (0);
// }

// void	builtin_unset(char **argv, t_env **envi)
// {
// 	t_env	*tmp;
// 	// t_env	*aaa;
// 	t_env 	**head;
// 	//faire l'env de base de env -i avec pwd shlvl etc...
// 	tmp = NULL;
// 	// aaa = NULL;
// 	head = envi;
// 	if (!head)
// 		return ;
// 	// if (search_env(argv[1], tmp->full == 1))
// 	// {
// 	// 	*envi = (*envi)->next;
// 	// 	free(tmp->full);
// 	// 	free(tmp);
// 	// 	return ;
// 	// }
// 	(void)argv;
// 	while ((*head)->next != NULL && search_env(argv[1], (*head)->next->full) == 0)
// 		head = &(*head)->next;
// 	// printf("deleted node = %s\n", (*head)->full);
// 	if (!*head && search_env(argv[1], (*head)->full) == 1)
// 	{
// 		tmp = *head;
// 		printf("deleted node = %s\n", (*head)->full);
// 		*head = tmp->next;
// 		free(tmp->full);
// 		free(tmp);
// 	}
// 	return ;
// }
	