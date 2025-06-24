/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:45:19 by gule-bat          #+#    #+#             */
/*   Updated: 2025/06/24 15:08:42 by gule-bat         ###   ########.fr       */
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

// void	builtin_unset(char **argv, t_env **envi)
// {
// 	t_env	*tmp;
// 	t_env	*aaa;

// 	//faire l'env de base de env -i avec pwd shlvl etc...
// 	tmp = *envi;
// 	aaa = NULL;
// 	if (tmp != NULL && ft_strcmp((*envi)->full, ""), argv[0], ft_strlen(argv[0]) == 0)
// 	{
// 		(*envi)= tmp->next;
// 		free(tmp);
// 	}
	
	
// 	return ;
// }
	