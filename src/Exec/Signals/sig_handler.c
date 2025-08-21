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

volatile sig_atomic_t g_sig_value = 0;

void    sig_dealer(int  signum, siginfo_t *info, void *context) //gives the value of the signal to the global variable to catch them
{
	(void)info;
	(void)context;
	if (signum == SIGINT)
	{
		printf("\n\n");
		g_sig_value = 128+SIGINT;
		rl_on_new_line();
		// printf("%s", PROMPT2);
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
		g_sig_value = SIGQUIT;
}

void    sig_handler(void)
{
	struct sigaction    signals;

	sigemptyset(&signals.sa_mask);
	signals.sa_sigaction = &sig_dealer;
	signals.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &signals, NULL) == -1)
		return ;
	signals.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &signals, NULL) == -1)
		return ;
	// if (sigaction(SIGSTOP, &signals, NULL) == -1)
		// return ;
}

pid_t	waiting_and_status(pid_t *pid, int *i)
{
	int stat;

	stat = 0;
	waitpid(*pid, &stat, 0);
	if (WIFEXITED(stat))
		g_sig_value = WEXITSTATUS(stat);
	else if (WIFSIGNALED(stat))
		g_sig_value = (128 + WTERMSIG(stat))%256;
	*i = 1;
	return (*pid);
}
