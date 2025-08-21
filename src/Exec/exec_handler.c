/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/05/16 17:40:51 by gule-bat          #+#    #+#             */
/*   Updated: 2025/05/16 17:40:51 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	subprocess(t_shell *shell, int *p_fd, int *fd, int bt)
{
	char	*path;

	path = NULL;
	if (((shell->cmd->next) && ((!shell->cmd->outfile))))
	{
		close(fd[0]);
		dupclose(&fd[1], STDOUT_FILENO);
	}
	else if (shell->cmd->next)
		close(fd[1]);
	if (shell->cmd->infile || shell->cmd->heredoc)
		if (*p_fd != STDIN_FILENO)
			close(*p_fd);
	redirs(&shell, p_fd, fd);
	if (bt == 1 || bt == 2)
		return (builtin_finder(&shell, is_builtin(shell->cmd->argv[0])),
			free_sub_proc(path, shell->env, shell->head, shell), exit(0), 1);
	path = pathing(shell->cmd, shell->env);
	if ((!shell->cmd->argv && !path) || g_sig_value == 1)
		return (free_sub_proc(path, shell->env, shell->head, shell), exit(1),
			-1);
	if (!path || execve(path, shell->cmd->argv, shell->envtab) < 0)
		return (perror("Minishell: Unable to execute command"),
			free_sub_proc(path, shell->env, shell->head, shell), exit(127), -1);
	exit(g_sig_value);
}

pid_t	processing(t_shell *shell, int *p_fd)
{
	int	bt;
	int	fd[2];

	bt = 0;
	if (shell->cmd->argv)
		bt = is_bt_int(is_builtin(shell->cmd->argv[0]));
	if ((bt == 1 && !shell->cmd->next) && *p_fd == STDIN_FILENO)
		return (builtin_finder(&shell, is_builtin(shell->cmd->argv[0])),
			shell->cmd->pid);
	if (piping(&shell, fd) == -1)
		return (-1);
	shell->cmd->pid = fork();
	if (shell->cmd->pid < 0)
		return (-1);
	if (shell->cmd->pid == 0)
		subprocess(shell, p_fd, fd, bt);
	if (*p_fd != STDIN_FILENO)
		close(*p_fd);
	if (shell->cmd->hd_fd > -1)
		close(shell->cmd->hd_fd);
	if (shell->cmd->next)
		return (close(fd[1]), fd[0]);
	else
		return (STDIN_FILENO);
}

int	process_waiter(t_cmd *head, t_shell *shell)
{
	pid_t	pidd;
	int		i;

	i = 0;
	(void)shell;
	if (!head)
		return (0);
	while (head && head->pid)
	{
		pidd = head->pid;
		while (!i)
			waiting_and_status(&pidd, &i);
		head = head->next;
		i = 0;
	}
	return (1);
}

int	process_board(t_shell *shell)
{
	int	err;
	int	p_fd;

	p_fd = -1;
	err = 0;
	p_fd = STDIN_FILENO;
	while (shell->cmd)
	{
		shell->in = &p_fd;
		p_fd = processing(shell, &p_fd);
		shell->cmd = shell->cmd->next;
	}
	process_waiter(shell->head, shell);
	return (err);
}

int	exec_handler(t_cmd *cmds, char **env, t_env **envi)
{
	t_shell	shell;

	shell = shell_init(cmds, env, envi);
	if (!cmds)
		return (0);
	if (checkerror(cmds) == 0)
		process_board(&shell);
	else
		g_sig_value = 2;
	if (shell.env)
		(*envi) = shell.env;
	else
		(*envi) = NULL;
	return (1);
}
