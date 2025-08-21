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

void	sigign(int val)
{
	close(STDIN_FILENO);
	g_sig_value = SIGINT;
	readline(NULL);
	// ft_putstr_fd("\r                      \n",2);
	(void)val;
}

int	dochere(int *fd, t_token *limiter, t_env *env)
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
	if ((!infile || infile == NULL))
		return (close(fd[0]), freetab(envv),
			ft_putstr_fd("\nhere-document delimited by end-of-file\n", 2),
			-1);
	if (((ft_strncmp(infile, limiter->value, ft_strlen(limiter->value)) == 0)
			&& infile[ft_strlen(limiter->value)] == '\n') || g_sig_value == SIGINT)
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
		g_sig_value = (128 + WTERMSIG(*stat)) % 256;
}

int	heredoc(t_stock *stock)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	pid = 0;
	status = -1;
	if (pipe((fd)) < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, &sigign);
		close(fd[0]);
		if (g_sig_value == SIGINT)
			return (close(fd[1]), free_cmds(stock->curr_cmd), free_tokens(stock->token_head),
				free_cmds(stock->cmd_head), free_env(stock->env), exit(g_sig_value), 0);
		while (1 || g_sig_value == SIGINT)
		{
			if (dochere(fd, stock->curr_token, stock->env) == -1 || g_sig_value == SIGINT)
				break ;
		}
		return (close(fd[1]), free_cmds(stock->curr_cmd), free_tokens(stock->token_head), 
			free_cmds(stock->cmd_head), free_env(stock->env), exit(g_sig_value), 0);
	}
	while (WIFEXITED(status) != 1)
		waiting_status(&pid, &status);
	return (close(fd[1]), fd[0]);
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

int	piping(t_shell **shell, int *fd)
{
	if ((*shell)->cmd->next)
	{
		if (pipe((fd)) == -1)
			return (-1);
	}
	else
	{
		fd[1] = STDOUT_FILENO;
		fd[0] = STDIN_FILENO;
	}
	return (0);
}
