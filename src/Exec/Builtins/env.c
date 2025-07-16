/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:45:19 by gule-bat          #+#    #+#             */
/*   Updated: 2025/07/14 04:02:37 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	nodesize(t_env *envi)
{
	int i;

	i = 0;
	if (!envi)
		return (maxascii);
	while (envi)
	{
		i++;
		envi = envi->next;
	}
	return (i);
}

void	builtin_env(t_shell *shell)
{
	t_env	*tmp;
	//faire l'env de base de env -i avec pwd shlvl etc...
	if (!shell->env)
		return ;
	tmp = shell->env;
	while (tmp)
	{
		if (shell->envtab == NULL && ft_strncmp(tmp->full, "PATH=", ft_strlen("PATH=")) == 0)
		{
			if (tmp->next)
				tmp = tmp->next;
			else
				break;
		}
		printf("%s\n", tmp->full);
		tmp = tmp->next;
	}
	return ;
}

int	search_env(char *line, char *baseline)
{
	int	i;
	char	*cpy;
	
	i = 0;
	cpy = NULL;
	if ((!line || !baseline))
		return (-1);
	cpy = ft_strdup(baseline);
	if (!cpy)
		return (free(cpy), -1);
	while (line[i] && line[i] != '=')
		i++;
	if ((int)ft_strlen(baseline) < i)
		return (free(cpy), -1);
	cpy[i] = '\0';
	printf("seqrchenv return 0 %s\n", line);
	printf("seqrchenv return 0 %s\n\n", cpy);
	if (ft_strcmp(line, cpy) != 0)
		return (free(cpy), -1);
	else
		return (free(cpy), 1);
}

// void	builtin_unset(t_shell *shell)
// {
// 	t_env	*tmp;
// 	t_env 	*head;
// 	//faire l'env de base de env -i avec pwd shlvl etc...
// 	tmp = NULL;
// 	head = NULL;
// 	head = shell->env;

// 	if (!shell->cmd->argv || !head)
// 		return ;
// 	if (head && search_env(shell->cmd->argv[1], (head)->full) == 1)
// 	{
// 		tmp = head;
// 		if (tmp->next)
// 		{
// 			head = tmp->next;
// 			free(tmp->full);
// 			free(tmp);
// 			shell->env = head;
// 			return ;
// 		}
// 		else if (!head->next)
// 		{
// 			free(tmp->full);
// 			free(tmp);
// 			shell->env = NULL;
// 			return ;
// 		}
// 	}
// 	while (head && head->next)
// 	{
// 		if (head && search_env(shell->cmd->argv[1], (head)->next->full) == 1)
// 		{
// 			if ((head)->next->next)
// 				tmp = (head)->next->next;
// 			// printf("deleted node = %s\n", tmp->full);
// 			free(head->next->full);
// 			free(head->next);
// 			head->next = tmp;
// 			// shell->env = head;
// 			return ;
// 		}
// 		head = head->next;
// 	}
// 	return ;
// }

void	builtin_unset(t_shell **shell)
{
	t_env	*tmp;
	t_env	*last;
	t_env	*head;

	last = NULL;
	head = (*shell)->env;	

	if (!(*shell)->cmd->argv || !head)
		return ;
	while (head)
	{
		if (search_env((*shell)->cmd->argv[1], (head)->full) == 1 && ft_strncmp((*shell)->cmd->argv[1], "_=", 1) != 0)
		{
			tmp = head;
			if (!last && (*shell)->env->next)
				(*shell)->env = (*shell)->env->next;
			else
				last->next = head->next;
			return (free(tmp->full), free(tmp));
		}
		last = head;
		head = head->next;
	}
	if (head == NULL)
		printf("unset: %s: invalid parameter name\n", (*shell)->cmd->argv[1]);
}
	