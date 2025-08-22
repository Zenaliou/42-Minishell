/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:24:34 by gule-bat          #+#    #+#             */
/*   Updated: 2025/05/17 14:24:34 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_sig_value = 0;

void	sig_dealer(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (signum == SIGINT && info->si_pid != 0)
	{
		printf("\n");
		g_sig_value = 128 + SIGINT;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT && info->si_pid != 0)
		g_sig_value = SIGQUIT;
}

void	sig_handler(void)
{
	struct sigaction	signals;

	sigemptyset(&signals.sa_mask);
	signals.sa_sigaction = &sig_dealer;
	signals.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &signals, NULL) == -1)
		return ;
	signals.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &signals, NULL) == -1)
		return ;
}

pid_t	waiting_and_status(pid_t *pid, int *i)
{
	int	stat;
	int	sig;

	stat = 0;
	waitpid(*pid, &stat, 0);
	if (WIFEXITED(stat))
		g_sig_value = WEXITSTATUS(stat);
	else if (WIFSIGNALED(stat))
	{
		sig = WTERMSIG(stat);
		g_sig_value = (128 + WTERMSIG(stat)) % 256;
		if (sig == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		else if (sig == SIGINT)
			ft_putstr_fd("\n", 2);
	}
	*i = 1;
	return (*pid);
}
/*	// else if (signum == SIGQUIT && info->si_pid == 0)
	// {
	// 	g_sig_value = SIGQUIT;
	// 	ft_putstr_fd("Quit (core dumped)", 2);
	// }*/