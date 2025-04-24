#include "../includes/minishell.h"

t_cmd *parse_tokens(t_token *tokens)
{
    t_cmd   *cmds;
    t_cmd   *current_cmd;

    cmds = NULL;
    current_cmd = NULL;

    if (!tokens)
        return (NULL);
        
    while (tokens)
    {
        if (tokens->type == WORD)
        {
            if (!current_cmd)
                current_cmd = new_cmd();
            if (current_cmd)
                add_argument(current_cmd, tokens->value);
        }
        else if (tokens->type == PIPE)
        {
            if (!current_cmd)
            {
                fprintf(stderr, "Syntax error: unexpected '|'\n");
                free_cmds(cmds);
                return (NULL);
            }
            add_cmd(&cmds, current_cmd);
            current_cmd = NULL;
        }
        else if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT || 
                 tokens->type == REDIR_APPEND || tokens->type == HEREDOC)
        {
            if (!current_cmd)
                current_cmd = new_cmd();
            if (!tokens->next || tokens->next->type != WORD)
            {
                fprintf(stderr, "Syntax error: missing file for redirection\n");
                free_cmds(cmds);
                if (current_cmd)
                    free_cmds(current_cmd);
                return (NULL);
            }
            if (tokens->type == REDIR_IN)
                current_cmd->infile = ft_strdup(tokens->next->value);
            else if (tokens->type == REDIR_OUT)
            {
                current_cmd->outfile = ft_strdup(tokens->next->value);
                current_cmd->append = 0;
            }
            else if (tokens->type == REDIR_APPEND)
            {
                current_cmd->outfile = ft_strdup(tokens->next->value);
                current_cmd->append = 1;
            }
            else if (tokens->type == HEREDOC)
            {
                current_cmd->heredoc = 1;
                current_cmd->limiter = ft_strdup(tokens->next->value);
            }
            tokens = tokens->next;
        }
        tokens = tokens->next;
    }
    if (current_cmd)
        add_cmd(&cmds, current_cmd);
    return (cmds);
}

/*TESTING FUNCTION*/

// void print_cmds(t_cmd *cmds)
// {
//     while (cmds)
//     {
//         printf("Command:\n");
//         for (int i = 0; cmds->argv && cmds->argv[i]; i++)
//             printf("  Arg[%d]: %s\n", i, cmds->argv[i]);
//         if (cmds->infile)
//             printf("  Input file: %s\n", cmds->infile);
//         if (cmds->outfile)
//             printf("  Output file: %s (append: %d)\n", cmds->outfile, cmds->append);
//         if (cmds->heredoc)
//             printf("  Heredoc limiter: %s\n", cmds->limiter);
//         cmds = cmds->next;
//     }
// }


// int main(void)
// {
//     char line[1024];
//     t_token *tokens;
//     t_cmd *cmds;

//     printf("Enter a command line: ");
//     if (fgets(line, sizeof(line), stdin) == NULL)
//     {
//         perror("Error reading input");
//         return (1);
//     }
//     size_t len = strlen(line);
//     if (len > 0 && line[len - 1] == '\n')
//         line[len - 1] = '\0';

//     tokens = tokenize(line);
//     if (!tokens)
//     {
//         printf("No tokens generated.\n");
//         return (1);
//     }
//     cmds = parse_tokens(tokens);
//     if (!cmds)
//     {
//         printf("Error parsing tokens.\n");
//         free_tokens(tokens);
//         return (1);
//     }
//     print_cmds(cmds);
//     free_tokens(tokens);
//     free_cmds(cmds);
//     return (0);
// }