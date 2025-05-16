#include "../includes/minishell.h"

void print_cmds(t_cmd *cmds)
{
    while (cmds)
    {
        printf("Command:\n");
        for (int i = 0; cmds->argv && cmds->argv[i]; i++)
            printf("  Arg[%d]: %s\n", i, cmds->argv[i]);
        if (cmds->infile)
            printf("  Input file: %s\n", cmds->infile);
        if (cmds->outfile)
            printf("  Output file: %s (append: %d)\n", cmds->outfile, cmds->append);
        if (cmds->heredoc)
            printf("  Heredoc limiter: %s\n", cmds->limiter);
        cmds = cmds->next;
    }
}

int main(int argc, char **argv, char **envp)
{
    char *line;
    t_token *tokens;
    t_cmd *cmds;

    (void)argc;
    (void)argv;
    printf("Entrez une commande : ");
    line = readline(NULL);
    if (!line)
    {
        printf("Aucune commande entrée.\n");
        return (1);
    }
    tokens = tokenize(line);
    expand_wildcards(tokens);
    expand_variables(tokens, envp);
    cmds = parse_tokens(tokens);
    printf("\n=== Résultat du parsing ===\n");
    print_cmds(cmds);
    free_tokens(tokens);
    free_cmds(cmds);
    free(line);
    return 0;
}
