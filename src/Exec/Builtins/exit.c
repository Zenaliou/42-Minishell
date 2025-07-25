#include "../includes/minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
    if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args, t_shell **shell)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!args[1])
	{
		free_sub_proc(NULL, (*shell)->env, (*shell)->head, *shell);
		exit(0);
	}
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", STDERR_FILENO);
		free_sub_proc(NULL, (*shell)->env, (*shell)->head, *shell);
		exit(255);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	free_sub_proc(NULL, (*shell)->env, (*shell)->head, *shell);
	exit(ft_atol(args[1]) % 256);
}