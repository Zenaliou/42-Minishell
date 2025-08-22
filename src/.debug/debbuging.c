#include "../includes/minishell.h"

void print_cmds_debug(t_cmd *cmds, int depth)
{
    while (cmds)
    {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("Command (depth %d):\n", depth);
        if (cmds->argv)
        {
            for (int i = 0; cmds->argv[i]; i++)
            {
                for (int j = 0; j < depth; j++)
                    printf("  ");
                printf("  Arg[%d]: %s\n", i, cmds->argv[i]);
            }
        }
        else
        {
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf("  No arguments\n");
        }
        if (cmds->infile)
        {
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf("  Input file: %s\n", cmds->infile);
        }
        if (cmds->outfile)
        {
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf("  Output file: %s (append: %d)\n", cmds->outfile, cmds->append);
        }
        if (cmds->heredoc)
        {
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf("  Heredoc limiter: %s\n", cmds->limiter);
        }
        if (cmds->append == 1)
        {
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf("  Operator: AND (&&)\n");
        }
        else if (cmds->append == 2)
        {
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf("  Operator: OR (||)\n");
        }
        if (cmds->left)
        {
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf("  Left subcommand:\n");
            print_cmds_debug(cmds->left, depth + 1);
        }
        if (cmds->right)
        {
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf("  Right subcommand:\n");
            print_cmds_debug(cmds->right, depth + 1);
        }
        if (cmds->err == 1 || cmds->err == 0)
        {
            for (int i = 0; i < depth; i++)
                printf("  ");
            if (cmds->err == 1)
                printf("  ERROR: ERROR IN LIST LIST NOT AVAILABLE\n");
        }
        
        cmds = cmds->next;
    }
}

void print_cmds(t_cmd *cmds)
{
    if (!cmds)
    {
        printf("No commands parsed.\n");
        return;
    }
    print_cmds_debug(cmds, 0);
}
