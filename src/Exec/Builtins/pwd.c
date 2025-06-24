/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 14:42:51 by niclee            #+#    #+#             */
/*   Updated: 2025/06/17 14:39:32 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_pwd(void)
{
	char	*str;

	str = getcwd(NULL, 0);
	if (!str)
		return ;
	printf("%s\n", str);
	free(str);
	return ;
}

void	builtin_cd(char **argv)
{
	if (!argv[1])
		chdir("~");
	else if (chdir(argv[1]) != 0)
		perror("cd : No such file or directory");
}