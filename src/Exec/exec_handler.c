/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:40:51 by gule-bat          #+#    #+#             */
/*   Updated: 2025/05/16 17:40:51 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t	waiting_and_status(pid_t *pid, int *i)
{
	int stat;

	stat = 0;
	waitpid(*pid, &stat, 0);
	if (WIFEXITED(stat))
		sig_value = WEXITSTATUS(stat);
	else if (WIFSIGNALED(stat))
		sig_value = (128 + WTERMSIG(stat))%256;
	// printf("\t\t\tSIG VALUE DEBUG  %d\n", sig_value);
	// printf("test status %d\n", WEXITSTATUS(stat));
	*i = 1;
	return (*pid);
}

void	dupclose(int *fd, int entry)
{
	dup2(*fd, entry);
	close(*fd);
}

char **list_to_tab(t_env *env)
{
	char	**tab;
	int	i;
	t_env	*tmp;
	t_env	*tmp2;

	tab = NULL;
	i = 0;
	tmp = env;
	tmp2 = env;
	while (tmp2)
	{	
		i++;
		tmp2 = tmp2->next;
	}
	tab = malloc((i+1) * sizeof(char *));
	ft_bzero(tab, (i+1) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (tmp)
	{
		tab[i++] = ft_strdup(tmp->full);
		tmp = tmp->next;
	}
	return (tab);
}

void	redirs(t_shell **shell, int *p_fd)
{
	int	fd;

	fd = 0;
	if (*p_fd != STDIN_FILENO)
		dupclose(p_fd, STDIN_FILENO);
	if ((*shell)->cmd->infile)
	{
		fd = open((*shell)->cmd->infile, O_RDONLY, 0644);
		if (fd >= 0)
			dupclose(&fd, STDIN_FILENO);
		else
			perror("Minishell: Unable to open file");
	}
	if ((*shell)->cmd->heredoc)
	{
		fd = heredoc(shell);
		if (fd >= 0)
			dupclose(&fd, STDIN_FILENO);
		close(fd);
		// if (!(*shell)->cmd->argv)
	}
	if ((*shell)->cmd->outfile)
	{
		if ((*shell)->cmd->append == 1)
			fd = open((*shell)->cmd->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else if ((*shell)->cmd->append == 0)
			fd = open((*shell)->cmd->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd < 0)
			perror ("Minishell: permission denied:");
		if (fd >= 0)
			dupclose(&fd, STDOUT_FILENO);
	}
}

pid_t	processing(t_shell *shell, int *p_fd)
{
	char	*path;
	int	bt; //builtin
	int		fd[2];

	bt = 0;
	path = NULL;
	// if (!shell->cmd || shell->cmd->heredoc == 1|| !shell->cmd->argv)
	// 	return -1;
	if (shell->cmd->argv)
		bt = is_bt_int(is_builtin(shell->cmd->argv[0]));
	if (bt == 1 && !shell->cmd->next)
		return (builtin_finder(&shell, is_builtin(shell->cmd->argv[0])), shell->cmd->pid);		// 2 == cd , 4 == export, 5 = unset, 7 == exit
	if (shell->cmd->next)
	{
		if (pipe((fd)) == -1)
			return (-1);
	}
	else
	{
		fd[1] = STDOUT_FILENO;
		fd[0] = STDIN_FILENO;
	}
	shell->cmd->pid = fork();
	if (shell->cmd->pid < 0)
		return (-1);
	if (shell->cmd->pid == 0)
	{
		ft_putstr_fd("SUBPROCESS\n", 2);
		redirs(&shell, p_fd);
		if ((shell->cmd->next) && (!shell->cmd->outfile || !shell->cmd->infile))
		{
			close(fd[0]);
			dupclose(&fd[1], 1);
		}
		// if (shell->cmd->heredoc && !shell->cmd->argv && !shell->cmd->next)
		// {
		// 	if (shell->cmd->next)
		// 		dup2(fd[1], STDOUT_FILENO);
		// 	print_heredoc(shell->cmd, path, &fd[0]);
		// 	// close(*p_fd);
		// 	// dupclose(&fd[0], 1);
		// 	dupclose(&fd[1], 0);

		// 	// close(fd[0]);
		// 	dupclose(&fd[1], 1);
		// 	free_sub_proc(NULL, shell->env, shell->head, shell);
		// 	exit(0);
		// }
		shell->envtab = list_to_tab(shell->env);
		if (bt == 1 || bt == 2)
		// 1 echo 3 pwd 6 env
			return (ft_putstr_fd("builtined subproc\n\n\n", 2), 
				builtin_finder(&shell, is_builtin(shell->cmd->argv[0])),
					free_sub_proc(path, shell->env, shell->head, shell), exit(0), 1);
		path = pathing(shell->cmd, shell->env);
		if (!shell->cmd->argv && !path)
			return (free_sub_proc(path, shell->env, shell->head, shell), exit(1), -1);
		if (!path || execve(path, shell->cmd->argv, shell->envtab) < 0)
			return (perror("Minishell: Unable to execute command"),	
				free_sub_proc(path, shell->env, shell->head, shell), exit(127), -1);
		exit(sig_value);
	}
	if (*p_fd != STDIN_FILENO)
		close(*p_fd);
	if (shell->cmd->next)
	{
		close(fd[1]);
		*p_fd = fd[0];
	}
	// dupclose(&fd[0], 0);
	if (shell->cmd->next)
		return (fd[0]);
	else
		return (STDIN_FILENO);
}

int	process_waiter(t_cmd *head, t_shell *shell)
{
	pid_t pidd;
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
	int p_fd;
	// int	i;
	
	p_fd = -1;
	err = 0;
	p_fd = STDIN_FILENO;
	while (shell->cmd)
	{
		shell->in = &p_fd;
		// if (shell->cmd->heredoc || shell->cmd->infile)
		// {
			// put subprocess hEredoc with dup, gnl etc...
			// p_fd = heredoc(&shell);
			// p_fd = other_input(shell, p_fd);
				// if (!shell->cmd->next)			
			// shell->cmd = shell->cmd->next;
		// }
		ft_putstr_fd("LOOP", 2);
		// if (((!shell->cmd->argv && shell->cmd->next) || !shell->cmd) && !shell->cmd->heredoc)
			// shell->cmd = shell->cmd->next;
		// else
		// {
		p_fd = processing(shell, &p_fd);
		shell->cmd = shell->cmd->next;
		// }
	}
	// dup2(p_fd, STDOUT_FILENO);
	process_waiter(shell->head, shell);
	printf("\n");
	return (err);
}

int	checkerror(t_cmd *cmds)
{
	t_cmd *tmp;

	if (!cmds)
		return (1);
	tmp = NULL;	
	tmp = cmds;
	while (tmp)
	{
		if (tmp->err == 1)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

t_shell	shell_init(t_cmd *cmds, char **env, t_env **envi)
{
	t_shell		shell;

	ft_bzero((void *)&shell, sizeof(shell));
	shell.cmd = cmds;
	shell.head = cmds;
	shell.env = (*envi);
	shell.envtab = NULL;
	shell.in = NULL;
	shell.out = NULL;
	if (!shell.envtab)
		shell.envtab = NULL;
	shell.line = NULL;
	(void)env;
	return (shell);
}


int exec_handler(t_cmd *cmds, char **env, t_env **envi)
{
	t_shell shell;

	shell = shell_init(cmds, env, envi);
	if (!cmds)
		return (0);
	if (!env)	//debug
		printf("-\tnoenv\n");	//debug
	if (checkerror(cmds) == 0)
		process_board(&shell);
	if (shell.env)
		(*envi) = shell.env;
	else
		(*envi) = NULL;
	// free_env(envi);
	return (1);
}
