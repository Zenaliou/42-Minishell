/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/07/31 18:24:17 by gule-bat          #+#    #+#             */
/*   Updated: 2025/07/31 18:24:17 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_finder(t_shell **shell, int i)
{
	if (!(*shell)->cmd->argv || !(*shell)->env)
		return ;
	if (i == 1)
		builtin_echo((*shell)->cmd->argv);
	else if (i == 2)
		builtin_cd((*shell)->cmd->argv, (*shell)->env);
	else if (i == 3)
		builtin_pwd();
	else if (i == 4)
		builtin_export(shell);
	else if (i == 5)
		builtin_unset(shell);
	else if (i == 6)
		builtin_env(*shell);
	else if (i == 7)
		builtin_exit((*shell)->cmd->argv, shell);
}

int	is_builtin(char *str)
{
	if (!str)
		return (-1);
	if (ft_strcmp(str, "echo") == 0)
		return (1);
	else if (ft_strcmp(str, "cd") == 0)
		return (2);
	else if (ft_strcmp(str, "pwd") == 0)
		return (3);
	else if (ft_strcmp(str, "export") == 0)
		return (4);
	else if (ft_strcmp(str, "unset") == 0)
		return (5);
	else if (ft_strcmp(str, "env") == 0)
		return (6);
	else if (ft_strcmp(str, "exit") == 0)
		return (7);
	else
		return (-1);
}

int	is_bt_int(int i)
{
	if (i == 2 || i == 4 || i == 5 || i == 7)
		return (1);
	else if (i == 1 || i == 3 || i == 6)
		return (2);
	else
		return (-1);
}

char	*go_to_equal(char *str)
{
	int		i;
	char	*dup;

	i = 0;
	dup = NULL;
	dup = ft_strdup(str);
	if (!dup)
		return (NULL);
	while (dup[i] && dup[i] != '=')
		i++;
	dup[i] = '\0';
	return (dup);
}
