/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:01:02 by gule-bat          #+#    #+#             */
/*   Updated: 2025/07/31 19:01:02 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int     dochere(char **infile, int *fd, char *limiter, t_shell *shell)
// {
// 	ft_putstr_fd("here_doc ->\t", 2);
// 	*infile = get_next_line(STDIN_FILENO);	
// 	if (!*infile || *infile == NULL)
// 	{
// 		close(fd[0]);
// 		close(fd[1]);
// 		// free_sub_proc(NULL, (shell)->env, (shell)->head, shell);
// 		exit(0);
// 	}
// 	if ((ft_strncmp(*infile, limiter, ft_strlen(limiter)) == 0)
// 		&& (*infile)[ft_strlen(limiter)] == '\n')
// 	{
// 		free(*infile);
// 		return (-1);
// 	}
// 	ft_putstr_fd(*infile, fd[1]);
// 	free(*infile);
// 	return (0);
// }

// void	waiting_status(pid_t *pid, int *stat)
// {
// 	waitpid(*pid, &(*stat), 0);
// 	if (WIFEXITED(*stat))
// 		g_sig_value = WEXITSTATUS(*stat);
// 	else if (WIFSIGNALED(*stat))
// 		g_sig_value = (128 + WTERMSIG(*stat))%256;
// 	// printf("\t\t\tSIG VALUE DEBUG  %d\n", g_sig_value);
// 	// printf("test status %d\n", WEXITSTATUS(stat));
// }

// int     heredoc(t_shell **shell)
// {
// 	char    *infile;
// 	int     fd[2];
// 	pid_t		pid;
// 	int		status;

// 	infile = NULL;
// 	pid = 0;
// 	status = -1;
// 	if (pipe((fd)) < 0)
// 		return (-1);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		// perror("SUBPROCESS HEREDOC\n");
// 		ft_putnbr_fd(pid, 2);
// 		close(fd[0]);
// 		while (1)
// 		{
// 			if (dochere(&infile, fd, (*shell)->cmd->limiter, (*shell)) == -1)
// 				break ;
// 		}
// 		close(fd[1]);
// 		free_sub_proc(NULL, (*shell)->env, (*shell)->head, *shell);
// 		exit(0);
// 	}
// 	// close(fd[0]);
// 	ft_putnbr_fd(pid, 2);
// 	while (WIFEXITED(status) != 1)
// 		waiting_status(&pid, &status);
// 	close(fd[1]);
// 	// if (/*!(*shell)->cmd->append || */!(*shell)->cmd->next && !(*shell)->cmd->append)
// 	// print_heredoc((*shell)->cmd, infile, &((*fd)[0]));
// 	// if ((*shell)->cmd->next && (!(*shell)->cmd->argv))
// 		// return (dup2(fd[0], 0), close(fd[0]), fd[0]);
// 	return (fd[0]);
// }

int     dochere(int *fd, t_token *limiter, t_env *env)
{
	char	*infile;
	char	*st;
	char	**envv;

	infile = NULL;
	st = NULL;
	envv = NULL;
	envv = list_to_tab(env);
	ft_putstr_fd("here_doc->", 2);
	infile = get_next_line(STDIN_FILENO);
	if (infile)
		st = expand_all_vars(infile, envv);
	if (!infile || infile == NULL)
		// close(fd[1]);
		return (close(fd[0]), freetab(envv), -2);
		// return (free_tokens(head), free_cmds(cmd), exit(0), -1);
	if ((ft_strncmp(infile, limiter->value, ft_strlen(limiter->value)) == 0)
		&& infile[ft_strlen(limiter->value)] == '\n')
		return (free(infile), free(st), freetab(envv), close(fd[0]), (-1));
	if (st)
		ft_putstr_fd(st, fd[1]);
	else
		ft_putstr_fd(infile, fd[1]);
	return (free(infile), free(st), freetab(envv), 0);
}

void	waiting_status(pid_t *pid, int *stat)
{
	waitpid(*pid, &(*stat), 0);
	if (WIFEXITED(*stat))
		g_sig_value = WEXITSTATUS(*stat);
	else if (WIFSIGNALED(*stat))
		g_sig_value = (128 + WTERMSIG(*stat))%256;
}

int     heredoc(t_stock *stock)
{
	int     fd[2];
	pid_t		pid;
	int		status;

	pid = 0;
	status = -1;
	if (pipe((fd)) < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		// ft_putnbr_fd(pid, 2);
		close(fd[0]);
		while (1)
		{
			if (dochere(fd, stock->curr_token, stock->env) == -1)
				break ;
			// else
				// return (close(fd[0]), close(fd[1]), free_tokens(head), free_cmds(cmd), free_env(env), exit(0), -1);
		}
		close(fd[1]);
		free_cmds(stock->curr_cmd);
		return (free_tokens(stock->token_head), free_cmds(stock->cmd_head), free_env(stock->env), exit(0), 0);
	}
	// ft_putnbr_fd(pid, 2);
	while (WIFEXITED(status) != 1)
		waiting_status(&pid, &status);
	close(fd[1]);
	return (fd[0]);
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
	close(*fd);
}