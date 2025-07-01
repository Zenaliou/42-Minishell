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
	while (env->next != NULL)
	{
		if (ft_strncmp(env->full, "PATH=", 4) == 0)
		{
			path = env->full;
			break;
		}
		else
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
	printf("l97 exec_handler\t pathchecker return (copy): %s\n", copy);
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

void	builtin_finder(t_shell *shell, int i)
{
	if (!shell->cmd->argv | !shell->env)
		return ;
	printf("-\tBUILTIN %d\n", i);
	if (i == 1)
		builtin_echo(shell->cmd->argv);
	else if (i == 2)
		builtin_cd(shell->cmd->argv, shell->env);
	else if (i == 3)
		builtin_pwd();
	// else if (i == 4)
		// builtin_export(envi);
	else if (i == 5)
		builtin_unset(shell->cmd->argv, &shell->env);
	else if (i == 6)
		builtin_env(shell->env);
	else if (i == 7)
		builtin_exit(shell->cmd->argv);	
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

pid_t	waiting_and_status(pid_t pid)
{
	int stat;

	stat = 0;
	waitpid(pid, &stat, 0);
	if (WIFEXITED(stat))
		sig_value = WEXITSTATUS(stat);
	else if (WIFSIGNALED(stat))
		sig_value = (128 + WTERMSIG(stat))%256;
	printf("\t\t\tSIG VALUE DEBUG  %d\n", sig_value);
	printf("test status %d\n", WEXITSTATUS(stat));
	return (pid);
}

void	free_sub_proc(char *path, t_env *envi, t_cmd *tmp)
{
	free_cmds(tmp);
	if (envi)
		free_env(envi);
	if (path)
		free(path);
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

pid_t	processing(t_shell *shell)
{
	// int stat;
	char	*path;

	path = NULL;
	// stat = 0;
	if (!shell->cmd || shell->cmd->heredoc == 1 /*|| shell->cmd->append */|| !shell->cmd->argv)
		return -1;
	if ((is_builtin(shell->cmd->argv[0]) == 2) || (is_builtin(shell->cmd->argv[0]) == 4) 
		|| (is_builtin(shell->cmd->argv[0]) == 5) || (is_builtin(shell->cmd->argv[0]) == 7))
		// 2 == cd , 4 == export, 5 = unset, 7 == exit
		return (printf("builtin parent\n\n\n"), builtin_finder(shell, is_builtin(shell->cmd->argv[0])), shell->cmd->pid);
	shell->cmd->pid = fork();
	if (shell->cmd->pid < 0)
		return (-1);
	if (shell->cmd->pid == 0)
	{
		printf("ICI MEME SUBPROCESS\n");
		// close(fd[0]);
		// dup2(fd[1], STDIN_FILENO);
		// close(fd[1]);
		path = pathing(shell->cmd, shell->env);
		if (is_builtin(shell->cmd->argv[0]) == 1 || is_builtin(shell->cmd->argv[0]) == 3 || is_builtin(shell->cmd->argv[0]) == 6)
		// 1 echo 3 pwd 6 env
		{
			printf("builtined subproc\n\n\n");
			builtin_finder(shell, is_builtin(shell->cmd->argv[0]));
			free_sub_proc(path, shell->env, shell->head);
			exit(0);
		}
		else if (!path || execve(path, shell->cmd->argv, shell->envtab) < 0)
		{
			perror("Minishell: Unable to execute command");
			free_sub_proc(path, shell->env, shell->head);
			exit(128);
		}
		exit(sig_value);
	}
	// waiting_and_status(pid);
	// dup2(fd[1], STDOUT_FILENO);
	// close(fd[1]);
	// close(fd[0]);
	return (waiting_and_status(shell->cmd->pid));
}

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

int	process_board(t_shell *shell)
{
	int	err;

	err = 0;
	while (shell->cmd)
	{
		if ((!shell->cmd->argv && shell->cmd->next) || !shell->cmd)
			shell->cmd = shell->cmd->next;
		if (shell->cmd->heredoc && shell->cmd->next)
		{
			// put subprocess heredoc with dup, gnl etc...
			shell->cmd = shell->cmd->next;
			//
		}
		else
		{
			err = processing(shell);
			shell->cmd = shell->cmd->next;
		}
	}
	// printf("-\tend process %d \n", shell->cmd->pid);
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

int exec_handler(t_cmd *cmds, char **env, t_env *envi)
{
	t_shell shell;

	ft_bzero((void *)&shell, sizeof(shell));
	shell.cmd = cmds;
	shell.head = cmds;
	shell.env = envi;
	shell.envtab = env;
	shell.line = NULL;
	if (!cmds)
		return (0);
	if (!env)	//debug
		printf("-\tnoenv\n");	//debug
	if (checkerror(cmds) == 0)
		process_board(&shell);
	// free_env(envi);
	return (1);
}

// FAIRE CD AVEC LE CD NEUTRE SUR ~