/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 23:43:36 by gule-bat          #+#    #+#             */
/*   Updated: 2025/07/31 23:43:36 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_sub_proc(char *path, t_env *envi, t_cmd *tmp, t_shell *shell)
{
	free_cmds(tmp);
	if (envi)
		free_env(envi);
	if (path)
		free(path);
	if ((shell)->envtab)
		freetab((shell)->envtab);
}
