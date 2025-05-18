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
