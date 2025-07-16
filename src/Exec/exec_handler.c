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

char	*ft_str_n_join(char *str, char *n, char *join)
{
	char	*res;
	int		i;
	int		j;
	int		size;

	if (!str || !n || !join)
		return (NULL);
	size = ft_strlen(str) + ft_strlen(n) + ft_strlen(join);
	res = malloc((size + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	ft_bzero(res, size + 1);
	while (str[i])
		res[j++] = str[i++];
	i = 0;
	while (n[i])
		res[j++] = n[i++];
	i = 0;
	while (join[i])
		res[j++] = join[i++];
	res[j + 1] = '\0';
	return (res);
}

char	**getenvmini(t_env *env)
{
	char	*path;
	char	**envd;
	int	i;

	i = 4;
	envd = NULL;
	path = NULL;
	if (!env)
		return (NULL);
	while (env != NULL && env->full)
	{
		if (ft_strncmp(env->full, "PATH=", 4) == 0)
		{
			path = env->full;
			break;
		}
		env = env->next;
	}
	if (!path)
		return (NULL);
	envd = ft_split(path+i, ':');
	if (!envd)
		return (free(path), NULL);
	return (envd);
}

char	*pathchecker(char *cmd, t_env *env)
{
	char	**envsym;
	char	*copy;
	int		i;

	i = 0;
	envsym = NULL;
	envsym = getenvmini(env);
	if (!envsym)
		return (NULL);
	while (envsym[i] != NULL)
	{
		// printf("copy path %s\n", envsym[i]); DEBUG
		copy = ft_str_n_join(envsym[i], "/", cmd);
		if (!copy)
			return (freetab(envsym), NULL);
		if (access(copy, F_OK | X_OK) == 0)
		{
			break ;
		}
		else if (envsym[i + 1] != NULL)
			free(copy);
		i++;
	}
	freetab(envsym);
	// printf("l97 exec_handler\t pathchecker return (copy): %s\n", copy);
	return (copy);
}

char	*pathing(t_cmd *cmds, t_env *env)
{
	char	*path;

	path = NULL;
	// if (!cmds->argv[0])
		// return (NULL);
	if (!cmds->argv)
		return (NULL);
	if (access(cmds->argv[0], F_OK | X_OK) != 0)
	{
		path = pathchecker(cmds->argv[0], env);
		if (!path)
			path = ft_strdup(cmds->argv[0]);
			// path = cmds->argv[0];
	}
	else
		// path = cmds->argv[0];
		path = ft_strdup(cmds->argv[0]);
	return (path);
}

void	builtin_finder(t_shell **shell, int i)
{
	printf("-\tBUILTIN %d\n", i);
	if (!(*shell)->cmd->argv | !(*shell)->env)
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

pid_t	waiting_and_status(pid_t *pid, int *i)
{
	int stat;

	stat = 0;
	waitpid(*pid, &stat, 0);
	if (WIFEXITED(stat))
		sig_value = WEXITSTATUS(stat);
	else if (WIFSIGNALED(stat))
		sig_value = (128 + WTERMSIG(stat))%256;
	printf("\t\t\tSIG VALUE DEBUG  %d\n", sig_value);
	// printf("test status %d\n", WEXITSTATUS(stat));
	*i = 1;
	return (*pid);
}

void	free_sub_proc(char *path, t_env *envi, t_cmd *tmp, t_shell *shell)
{
	free_cmds(tmp);
	if (envi)
		free_env(envi);
	if (path)
		free(path);
	if ((shell)->envtab)
		freetab((shell)->envtab);
}

// pid_t	processing(char	*path, char **env, t_cmd *cmds, t_env *envi)
// {
// 	pid_t	pid;
// 	// int	fd[2];
// 	int stat;
// 	static t_cmd *tmp;

// 	// if (pipe(fd) == -1)
// 	// 	return (-1);
// 	if (!tmp)
// 		tmp = cmds;
// 	pid = -1;
// 	stat = 0;
// 	if (!path)
// 		return -1;
// 	if ((is_builtin(cmds->argv[0]) == 2) || (is_builtin(cmds->argv[0]) == 4) 
// 		|| (is_builtin(cmds->argv[0]) == 5) || (is_builtin(cmds->argv[0]) == 7))
// 		// 2 == cd , 4 == export, 5 = unset, 7 == exit
// 		return (printf("builtin parent\n\n\n"), builtin_finder(shell->cmds->argv, &envi, is_builtin(shell->cmds->argv[0]), &cmds), pid);
// 	pid = fork();
// 	if (pid < 0)
// 		return (-1);
// 	if (pid == 0)
// 	{
// 		printf("ICI MEME SUBPROCESS\n");
// 			// close(fd[0]);
// 			// dup2(fd[1], STDIN_FILENO);
// 			// close(fd[1]);
// 			if (is_builtin(cmds->argv[0]) == 1 || is_builtin(cmds->argv[0]) == 3 || is_builtin(cmds->argv[0]) == 6) 
// 			// 1 echo 3 pwd 6 env
// 			{
// 				printf("builtined subproc\n\n\n");
// 				builtin_finder(cmds->argv, &envi, is_builtin(cmds->argv[0]), &cmds);
// 				free_sub_proc(path, envi, tmp);
// 				exit(0);
// 			}
// 			else if (execve(path, cmds->argv, env) < 0)
// 			{
// 				perror("Minishell: Unable to execute command");
// 				free_sub_proc(path, envi, tmp);
// 				exit(128);
// 			}
// 			exit(sig_value);
// 	}
// 	waiting_and_status(pid);
// 	// dup2(fd[1], STDOUT_FILENO);
// 	// close(fd[1]);
// 	// close(fd[0]);
// 	return (waiting_and_status(pid));
// }

int is_bt_int(int i)
{
	if (i == 2 || i == 4 || i == 5 || i == 7)
		return (1);
	else if (i == 1 || i == 3 || i == 6)
		return (2);
	else
		return (-1);
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

pid_t	processing(t_shell *shell)
{
	char	*path;
	int	bt; //builtin
	// int		pipe[2];

	bt = 0;
	path = NULL;
	// shell->fd = pipe;
	if (!shell->cmd || shell->cmd->heredoc == 1 /*|| shell->cmd->append */|| !shell->cmd->argv)
		return -1;
	bt = is_bt_int(is_builtin(shell->cmd->argv[0]));
	if (bt == 1)
		// 2 == cd , 4 == export, 5 = unset, 7 == exit
		return (builtin_finder(&shell, is_builtin(shell->cmd->argv[0])), shell->cmd->pid);
	// if (shell->cmd->next)
	// 	if (pipe(pipe) == -1)
	// 		return (-1);
	shell->cmd->pid = fork();
	if (shell->cmd->pid < 0)
		return (-1);
	if (shell->cmd->pid == 0)
	{

		shell->envtab = list_to_tab(shell->env);
		printf("SUBPROCESS\n");
		if (bt == 2)
		// 1 echo 3 pwd 6 env
		{
			printf("builtined subproc\n\n\n");
			builtin_finder(&shell, is_builtin(shell->cmd->argv[0]));
			free_sub_proc(path, shell->env, shell->head, shell);
			exit(0);
		}
		path = pathing(shell->cmd, shell->env);
		if (!path || execve(path, shell->cmd->argv, shell->envtab) < 0)
		{
			perror("Minishell: Unable to execute command");
			free_sub_proc(path, shell->env, shell->head, shell);
			exit(128);
		}
		exit(sig_value);
	}
	// dup2(fd[1], STDOUT_FILENO);
	// close(fd[1]);
	// close(fd[0]);
	return (/*waiting_and_status(shell->cmd->pid)*//*   FD   */ -1);
}

void	dupclose(int *fd, int entry)
{
	dup2(*fd, entry);
	close(*fd);
}

// pid_t	processing(t_shell *shell)
// {
// 	char	*path;
// 	int	bt; //builtin
// 	// int		pipe[2];

// 	bt = 0;
// 	path = NULL;
// 	// shell->fd = pipe;
// 	if (!shell->cmd || shell->cmd->heredoc == 1 /*|| shell->cmd->append */|| !shell->cmd->argv)
// 		return -1;
// 	bt = is_bt_int(is_builtin(shell->cmd->argv[0]));
// 	if (bt == 1)
// 		// 2 == cd , 4 == export, 5 = unset, 7 == exit
// 		return (builtin_finder(&shell, is_builtin(shell->cmd->argv[0])), shell->cmd->pid);
// 	// if (shell->cmd->next)
// 	// 	if (pipe(pipe) == -1)
// 	// 		return (-1);
// 	shell->cmd->pid = fork();
// 	if (shell->cmd->pid < 0)
// 		return (-1);
// 	if (shell->cmd->pid == 0)
// 	{
// 		shell->envtab = list_to_tab(shell->env);
// 		if (shell->fd[0] != -1)
// 			dupclose(&shell->fd[0], STDIN_FILENO);	
// 		if (shell->fd[1] != -1)
// 			dupclose(&shell->fd[1], STDOUT_FILENO);	
// 		// printf("SUBPROCESS\n");
// 		if (bt == 2)
// 		// 1 echo 3 pwd 6 env
// 		{
// 			// printf("builtined subproc\n\n\n");
// 			builtin_finder(&shell, is_builtin(shell->cmd->argv[0]));
// 			free_sub_proc(path, shell->env, shell->head, shell);
// 			exit(0);
// 		}
// 		path = pathing(shell->cmd, shell->env);
// 		if (!path || execve(path, shell->cmd->argv, shell->envtab) < 0)
// 		{
// 			perror("Minishell: Unable to execute command");
// 			free_sub_proc(path, shell->env, shell->head, shell);
// 			if (shell->fd[0] != -1)
// 				close(shell->fd[0]);	
// 			if (shell->fd[1] != -1)
// 				close(shell->fd[1]);	
// 			exit(128);
// 		}
// 		exit(sig_value);
// 	}
// 	// dup2(fd[1], STDOUT_FILENO);
// 	// close(fd[1]);
// 	// close(fd[0]);
// 	return (/*waiting_and_status(shell->cmd->pid)*//*   FD   */ shell->fd[0]);
// }
//VERSION PIPES


// || PREMIER FONCTIONNEL A PRINT, SI COMMAND NON EXISTANTE RETURN ERROR D'EXEC TT DE MM
// && TT FONCTIONNEL OU RIEN; TOUT SUR LA SORTIE DEMANDE SI CA MARCHE OU RETURN NOT FOUND
// int	process_board(t_cmd *cmds, t_env *envi, char **env)
// {
// 	t_cmd	*temp;
// 	char	*path;
// 	pid_t	pid;
	
// 	path = NULL;
// 	temp = cmds;
// 	pid = -1;
// 	while (cmds)
// 	{
// 		path = pathing(cmds, envi);
// 		printf("path processboard 259 %s\n", path);
// 		pid = processing(path, env, cmds, envi);
// 		// if (access(cmds->argv[0], F_OK | X_OK) != 0 && path)
// 			free(path);
// 		// if (cmds->next)
// 		cmds = cmds->next;
// 		// if (temp)
// 			// free_cmd(temp);
// 		// else if (cmds->left)
// 			// cmds = cmds->left;
// 		// else if (cmds->right)
// 			// cmds = cmds->right;
		
// 	}
// 	printf("-\tend process %d \n", pid);
// 	printf("\n");
// 	// free_cmds(temp);
// 	return (pid);
// }
// void    sig_dea(int  signum, siginfo_t *info, void *context) //gives the value of the signal to the global variable to catch them
// {
// 	(void)info;
// 	(void)context;
// 	if (signum == SIGINT && (info->si_pid == 0 && sig_value == 467982))
// 	{
// 		ft_putstr_fd("\n\nHeredoc:\nPress enter to continue, EOF or Ctrl+d to stop\n", 2);
// 		return ;
// 	}
// }

// void	dodgesig_heredoc()
// {
// 	struct sigaction sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_handler = SIG_DFL;
// 	sa.sa_sigaction = &sig_dea;
// 	sa.sa_flags = SA_RESTART;
// 	sigaction(SIGINT, &sa, NULL);
// }

int     dochere(char **infile, int *fd, char *limiter, t_shell *shell)
{
	ft_putstr_fd("here_doc ->\t", 2);
	*infile = get_next_line(STDIN_FILENO);	
	if (!*infile || *infile == NULL)
	{
		close(fd[0]);
		close(fd[1]);
		free_sub_proc(NULL, (shell)->env, (shell)->head, shell);
		exit(0);
	}
	if ((ft_strncmp(*infile, limiter, ft_strlen(limiter)) == 0)
		&& (*infile)[ft_strlen(limiter)] == '\n')
	{
		free(*infile);
		return (-1);
	}
	ft_putstr_fd(*infile, fd[1]);
	free(*infile);
	return (0);
}

void	print_heredoc(t_cmd *cmd, char *infile, int *fd)
{
	while (1 && (!cmd->append))
	{
		infile = get_next_line(*fd);
		if (!infile)
			break ;
		ft_putstr_fd(infile, STDOUT_FILENO);
		free(infile);
	}
}

int     heredoc(t_shell **shell)
{
	char    *infile;
	int             fd[2];

	infile = NULL;
	if (pipe(fd) < 0)
		return (-1);
	(*shell)->cmd->pid = fork();
	if ((*shell)->cmd->pid == 0)
	{
		sig_value = 467982;
		// signal(SIGINT, SIG_DFL);
		// signal(SIGQUIT, SIG_DFL);
		// dodgesig_heredoc();
		close(fd[0]);
		sig_value = 467982;
		while (1)
		{
			if (dochere(&infile, fd, (*shell)->cmd->limiter, (*shell)) == -1)
				break ;
		}
		close(fd[1]);
		free_sub_proc(NULL, (*shell)->env, (*shell)->head, *shell);
		exit(0);
	}
	printf("%d\n", (*shell)->cmd->pid);
	close(fd[1]);
	wait(NULL);
	// waitpid((*shell)->cmd->pid, NULL, 0);
	if (/*!(*shell)->cmd->append || */!(*shell)->cmd->next && !(*shell)->cmd->append)
		print_heredoc((*shell)->cmd, infile, &fd[0]);
	close(fd[0]);
	return (fd[0]);
}


int	process_waiter(t_cmd *head, t_shell *shell)
{
	pid_t pidd;
	int		i;
	// char	*b;

	i = 0;
	(void)shell;
	// b = NULL;
	if (!head)
		return (0);
	// if (head->heredoc)
		// return (waiting_and_status(&pidd, &i));
	// process_waiter(head);
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
	// int fd[2];
	// fd_in = -1;
	// fd_out = -1;
	err = 0;
	while (shell->cmd)
	{
		if (shell->cmd->heredoc)
		{
			// put subprocess heredoc with dup, gnl etc...
			/*fd[0] = */heredoc(&shell);
				// if (!shell->cmd->next)			
			shell->cmd = shell->cmd->next;
		}
		else if ((!shell->cmd->argv && shell->cmd->next) || !shell->cmd)
			shell->cmd = shell->cmd->next;
		else
		{
			/*fd[0] = */processing(shell);
			shell->cmd = shell->cmd->next;
		}
	}
	process_waiter(shell->head, shell);
	printf("\n");
	return (err);
}

// int	process_board(t_shell *shell)
// {
// 	int	err;
// 	int fd[2];
// 	int last;

// 	// fd_in = -1;
// 	// fd_out = -1;
// 	err = 0;
// 	last = -1;
// 	shell->fd = fd;
// 	while (shell->cmd)
// 	{
// 		if (shell->cmd->heredoc)
// 		{
// 			// put subprocess heredoc with dup, gnl etc...
// 			if (last != -1)
// 				close(last);
// 			last = heredoc(&shell);
// 				// if (!shell->cmd->next)			
// 		}
// 		else if ((!shell->cmd->argv && shell->cmd->next) || !shell->cmd)
// 			last = last;
// 		else 
// 		{
// 			if (shell->cmd->next)
// 			{
// 				if (pipe(shell->fd) == -1)
// 					return (-1);
// 				shell->fd[1] = fd[1];
// 			}
// 			else
// 				shell->fd[1] = -1;
// 			shell->fd[0] = last;
// 			if (shell->cmd->next)
// 				last = fd[0];
// 			else
// 				last = -1;
//   			fd[0] = processing(shell);
// 			if (shell->cmd->pid == -1)
// 				last = -1;
// 			else if (shell->cmd->next)
// 				last = fd[0]
// 			if (shell->fd[0] != -1)
// 				close(shell->fd[0]);	
// 			if (shell->fd[1] != -1)
// 				close(shell->fd[1]);
// 		}
// 		shell->cmd = shell->cmd->next;
// 	}
// 	process_waiter(shell->head, shell);
// 	printf("\n");
// 	return (err);
// }


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
	shell.fd = NULL;
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
	// ft_bzero((void *)&shell, sizeof(shell));
	// shell.cmd = cmds;
	// shell.head = cmds;
	// shell.env = (*envi);
	// shell.envtab = env;
	// if (!shell.envtab)
	// 	shell.envtab = NULL;
	// shell.line = NULL;
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

// int exec_handler(t_shell	*shell)
// {

// 	ft_bzero((void *)&shell, sizeof(shell));
// 	shell.cmd = cmds;
// 	shell.head = cmds;
// 	if (!shell.env)
// 		shell.env = (*envi);
// 	shell.envtab = env;
// 	if (!shell.envtab)
// 		shell.envtab = NULL;
// 	shell.line = NULL;
// 	if (!cmds)
// 		return (0);
// 	if (!env)	//debug
// 		printf("-\tnoenv\n");	//debug
// 	if (checkerror(cmds) == 0)
// 		process_board(&shell);
// 	// free_env(envi);
// 	return (1);
// }

// FAIRE CD AVEC LE CD NEUTRE SUR ~