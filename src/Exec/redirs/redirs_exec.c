/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:09:22 by gule-bat          #+#    #+#             */
/*   Updated: 2025/08/21 18:31:53 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	redir_in(t_shell **shell, int *fd)
{
	*fd = open((*shell)->cmd->infile, O_RDONLY, 0644);
	if (*fd >= 0)
		dupclose(&(*fd), STDIN_FILENO);
	else
	{
		*fd = open("/dev/null", O_RDONLY, 0644);
		dupclose(&(*fd), STDIN_FILENO);
		perror("Minishell: Unable to open file");
	}
}

void	redir_out(t_shell **shell, int *fd)
{
	if ((*shell)->cmd->append == 1)
		*fd = open((*shell)->cmd->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if ((*shell)->cmd->append == 0)
		*fd = open((*shell)->cmd->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (*fd < 0)
	{
		perror ("Minishell: ");
		g_sig_value = 1;
		*fd = open("/dev/null", O_WRONLY);
	}
	if (*fd >= 0)
		dupclose(&(*fd), STDOUT_FILENO);
}

void	redirs(t_shell **shell, int *p_fd, int *s_fd)
{
	int	fd;

	fd = -1;
	(void)s_fd;
	if (*p_fd != STDIN_FILENO
		&& (!((*shell)->cmd->heredoc) || !(*shell)->cmd->infile))
		dupclose(p_fd, STDIN_FILENO);
	else if (*p_fd != STDIN_FILENO)
		close(*p_fd);
	if ((*shell)->cmd->infile)
		redir_in(shell, &fd);
	if ((*shell)->cmd->heredoc)
	{
		fd = (*shell)->cmd->hd_fd;
		if ((fd >= 0))
			dupclose(&fd, STDIN_FILENO);
		close((*shell)->cmd->hd_fd);
	}
	if ((*shell)->cmd->outfile && (*shell)->cmd->argv)
		redir_out(shell, &fd);
	(*shell)->envtab = list_to_tab((*shell)->env);
}

void	dupclose(int *fd, int entry)
{
	dup2(*fd, entry);
	close(*fd);
}
