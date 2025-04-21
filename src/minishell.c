#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    char *line;
    t_token *tokens;
    t_cmd *cmds;

    (void)argc;
    (void)argv;
    while (1)
    {
        line = readline("minishell> ");
        if (!line)
        {
            printf("exit\n");
            break;
        }
        if (*line)
            add_history(line);
        tokens = tokenize(line);
        expand_wildcards(tokens);
        expand_variables(tokens, envp);
        cmds = parse_tokens(tokens);
        execute_commands(cmds, envp);
        free_tokens(tokens);
        free_cmds(cmds);
        free(line);
    }
    return 0;
}
