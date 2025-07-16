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

volatile sig_atomic_t sig_value = 0;

void    sig_dealer(int  signum, siginfo_t *info, void *context) //gives the value of the signal to the global variable to catch them
{
	(void)info;
	(void)context;
	if (signum == SIGINT)
	{
		if (sig_value == 467982)
			return ;
		printf("\n\n");
		sig_value = 128+SIGINT;
		rl_on_new_line();
		printf("%s", prompt2);
		// rl_replace_line(prompt2, 0);
		rl_replace_line("", 0);
		rl_redisplay();
		// rl_replace_line("", 0);
	}
	else if (signum == SIGQUIT)
		sig_value = SIGQUIT;
}

// void    sig_dealer_begin(int  signum, siginfo_t *info, void *context) //gives the value of the signal to the global variable to catch them
// {
// 	(void)info;
// 	(void)context;
// 	if (signum == SIGINT)
// 	{
// 		printf("\n");
// 		sig_value = 128+SIGINT;
// 	}
// }

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