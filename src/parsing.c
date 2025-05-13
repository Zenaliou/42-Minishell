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
        else if (tokens->type == LPAREN)
        {
            tokens = tokens->next; // Passer la parenthèse ouvrante
            t_cmd *sub_cmd = parse_tokens(tokens); // Appel récursif pour analyser la sous-commande
            if (!sub_cmd)
            {
                fprintf(stderr, "Syntax error: unmatched '('\n");
                free_cmds(cmds);
                return (NULL);
            }
            if (!current_cmd)
                current_cmd = new_cmd();
            current_cmd->left = sub_cmd; // Associer la sous-commande
            while (tokens && tokens->type != RPAREN)
                tokens = tokens->next; // Avancer jusqu'à la parenthèse fermante
            if (!tokens || tokens->type != RPAREN)
            {
                fprintf(stderr, "Syntax error: unmatched '('\n");
                free_cmds(cmds);
                return (NULL);
            }
        }
        else if (tokens->type == AND || tokens->type == OR)
        {
            if (!current_cmd)
            {
                fprintf(stderr, "Syntax error: unexpected '%s'\n", tokens->value);
                free_cmds(cmds);
                return (NULL);
            }
            t_cmd *new_cmds = new_cmd();
            new_cmds->left = current_cmd; // Associer la commande courante comme sous-commande gauche
            new_cmds->append = (tokens->type == AND) ? 1 : 2; // 1 pour AND, 2 pour OR, norminette error
            current_cmd = new_cmds;

            tokens = tokens->next; // Passer l'opérateur logique
            if (tokens->type == LPAREN)
            {
                tokens = tokens->next; // Passer la parenthèse ouvrante
                t_cmd *sub_cmd = parse_tokens(tokens); // Appel récursif pour analyser la sous-commande
                if (!sub_cmd)
                {
                    fprintf(stderr, "Syntax error: unmatched '('\n");
                    free_cmds(cmds);
                    return (NULL);
                }
                current_cmd->right = sub_cmd; // Associer la sous-commande droite
                while (tokens && tokens->type != RPAREN)
                    tokens = tokens->next; // Avancer jusqu'à la parenthèse fermante
                if (!tokens || tokens->type != RPAREN)
                {
                    fprintf(stderr, "Syntax error: unmatched '('\n");
                    free_cmds(cmds);
                    return (NULL);
                }
            }
            else
            {
                t_cmd *right_cmd = parse_tokens(tokens); // Analyser la commande droite
                if (!right_cmd)
                {
                    fprintf(stderr, "Syntax error: missing command after '%s'\n", tokens->value);
                    free_cmds(cmds);
                    return (NULL);
                }
                current_cmd->right = right_cmd;
            }
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