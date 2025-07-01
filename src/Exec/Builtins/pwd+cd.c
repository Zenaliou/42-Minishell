/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd+cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 14:42:51 by niclee            #+#    #+#             */
/*   Updated: 2025/07/01 21:53:45 by gule-bat         ###   ########.fr       */
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

char	*get_env_remake(t_env *envi)
{
	char *pwd;
	char *path;
	int		i;

	i = 0;
	pwd = NULL;
	path = NULL;
	while (envi && ft_strncmp(envi->full, "PWD=/", 3) != 0)
		envi = envi->next;
	pwd = envi->full;
	while (pwd[i] != '=')
		i++;
	i++;
	path = ft_strdup(&pwd[i]);
	printf("pwd from env %s\n", path);
	return (path);
}


char	*get_home(t_env *envi)
{
	char	*str;
	int		i;
	int		j;

	str = NULL;
	i = 0;
	j = 0;
	str = get_env_remake(envi);
	(void)envi;
	while (str[i] != '\0')
		i++;
	if (i > 1)
	{
		i = 0;
		while (str[i] && j <= 3)
		{
			if (str[i] == '/')
				j++;
			if (j == 3)
			{
				str[i+1] = '\0';
				break ;
			}
			i++;
			
		}
	}
	printf("path cd %s\n", str);
	return (str);
}

void	builtin_cd(char **argv, t_env *envi)
{
	char	*str;

	str = NULL;
	if ((!argv[1]) || (ft_strcmp(argv[1], "~") == 0))
	{
		str = get_home(envi);
		if (chdir(str) != 0)
			perror("cd : No such file or directory");
		free(str);
		return ;
	}
	if (ft_strcmp(argv[1], "-") == 0)
	{
		str = get_env_remake(envi);
		if (chdir(str))
			perror("cd : no such file or directory");
		free(str);
	}
	else if (chdir(argv[1]) != 0)
		perror("cd : No such file or directory");
}