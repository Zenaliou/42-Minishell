/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:11:58 by marvin            #+#    #+#             */
/*   Updated: 2025/05/19 21:11:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	j;
	int	n_option;

	n_option = 0;
	i = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		n_option = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i++ + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (!n_option)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

// int	main(int ac, char **av)
// {
// 	(void)ac;
// 	builtin_echo(av);
// 	return (0);
// }