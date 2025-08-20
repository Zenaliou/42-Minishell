/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:57:34 by niclee            #+#    #+#             */
/*   Updated: 2025/08/20 16:57:07 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../includes/minishell.h"

void	parsing_n_expand(char *line, char **env, t_cmd **cmds, t_env *envi)
{
	t_token	*tokens;
	char	**envv;

	(void)env;
	tokens = NULL;
	tokens = tokenize(line);
	expand_wildcards(tokens);
	envv = list_to_tab(envi);
	expand_variables(tokens, envv);
	freetab(envv);
	*cmds = parse_tokens(tokens, envi);
	// ft_putstr_fd("\n=== Résultat du parsing ===\n", 2); // DEBUG
	// print_cmds(*cmds); // DEBUG
	free_tokens(tokens);
	free(line);	
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char	*line;
// 	t_cmd	*cmds;
// 	t_env	*envi;
// 	// t_shell	shell;

// 	(void)argc;
// 	(void)argv;
// 	envi = NULL;
// 	cmds = NULL;
// 	fakeenv(envp, &envi);
// 	sig_handler();
// 	while (1)
// 	{
// 		line = readline(prompt); // texte trop long donc dans le .h
// 		if (!line)
// 			return (printf("exit\n"), free(line), free_env(envi), 0);
// 				// raccourci
// 		if (*line)
// 			add_history(line);
// 		parsing_n_expand(line, envp, &cmds, envi);
// 		if (cmds && checkerror(cmds) == 0)
// 			// exec_handler(&shell);
// 			exec_handler(cmds, envp, &envi);
// 		free_cmds(cmds);
// 	}
// 	free_env(envi);
// 	free(line);
// 	rl_clear_history();
// 	return (0);
// }

int	main(int argc, char **argv, char **envp) // tester main
{
	char	*line = NULL;
	t_cmd	*cmds;
	t_env	*envi;

	(void)argc;
	(void)argv;
	envi = NULL;
	cmds = NULL;
	fakeenv(envp, &envi);
	sig_handler();
	if (argc == 2)
	{
		line = ft_strdup(argv[1]);
		parsing_n_expand(line, envp, &cmds, envi);
		if (cmds && cmds->err == 0)
			// exec_handler(&shell);
			exec_handler(cmds, envp, &envi);
		free_cmds(cmds);
		free_env(envi);
		// free(line);
	}
	else
	{
		while (1)
		{
			line = readline(prompt); // texte trop long donc dans le .h
			if (!line)
				return (printf("exit\n"), free(line), free_env(envi), 0);
					// raccourci
			if (*line)
				add_history(line);
			parsing_n_expand(line, envp, &cmds, envi);
			if (cmds && cmds->err == 0)
				// exec_handler(&shell);
				exec_handler(cmds, envp, &envi);
			else if (cmds && cmds->err != 0)
				sig_value = cmds->err;
			free_cmds(cmds);
		}
		free_env(envi);
		free(line);
		rl_clear_history();
	}
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
