/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:57:34 by niclee            #+#    #+#             */
/*   Updated: 2025/07/13 18:07:34 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../includes/minishell.h"

void	parsing_n_expand(char *line, char **env, t_cmd **cmds)
{
	t_token	*tokens;

	tokens = NULL;
	tokens = tokenize(line);
	expand_wildcards(tokens);
	expand_variables(tokens, env);
	*cmds = parse_tokens(tokens);
	printf("\n=== Résultat du parsing ===\n"); // DEBUG
	print_cmds(*cmds);
	free_tokens(tokens);
	free(line);	
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_cmd	*cmds;
	t_env	*envi;
	// t_shell	shell;

	(void)argc;
	(void)argv;
	envi = NULL;
	cmds = NULL;
	fakeenv(envp, &envi);
	sig_handler();
	while (1)
	{
		line = readline(prompt); // texte trop long donc dans le .h
		if (!line)
			return (printf("exit\n"), free(line), free_env(envi), 0);
				// raccourci
		if (*line)
			add_history(line);
		parsing_n_expand(line, envp, &cmds);
		if (cmds && cmds->err == 0)
			// exec_handler(&shell);
			exec_handler(cmds, envp, &envi);
		free_cmds(cmds);
	}
	free_env(envi);
	free(line);
	rl_clear_history();
	return (0);
}

// int main(int argc, char **argv, char **envp)
// {
//     char *line;
//     t_token *tokens;
//     t_cmd *cmds;

//     (void)argc;
//     (void)argv;
//     while (1)
//     {
//         line = readline("minishell> ");
//         if (!line)
//         {
//             printf("exit\n");
//             break ;
//         }
//         if (*line)
//             add_history(line);
//         tokens = tokenize(line);
//         expand_wildcards(tokens);
//         expand_variables(tokens, envp);
//         cmds = parse_tokens(tokens);
//         printf("\n=== Résultat du parsing ===\n");
//         print_cmds(cmds);
//         execute_commands(cmds, envp);
//         free_tokens(tokens);
//         free_cmds(cmds);
//         free(line);
//     }
//     return (0);
// }
