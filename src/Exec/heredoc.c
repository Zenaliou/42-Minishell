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

int     heredoc(t_shell **shell)
{
	char    *infile;
	int     fd[2];
	pid_t		pid;

	infile = NULL;
	pid = 0;
	if (pipe((fd)) < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		perror("SUBPROCESS HEREDOC\n");
		close(fd[0]);
		while (1)
		{
			if (dochere(&infile, fd, (*shell)->cmd->limiter, (*shell)) == -1)
				break ;
		}
		close(fd[1]);
		free_sub_proc(NULL, (*shell)->env, (*shell)->head, *shell);
		exit(0);
	}
	ft_putnbr_fd(pid, 2);
	waitpid(pid, NULL, 0);
	close(fd[1]);
	// if (/*!(*shell)->cmd->append || */!(*shell)->cmd->next && !(*shell)->cmd->append)
	// print_heredoc((*shell)->cmd, infile, &((*fd)[0]));
	// dup2(fd[0], 0);
	// close(fd[0]);
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