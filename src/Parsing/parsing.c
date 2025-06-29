#include "../includes/minishell.h"

/*
Objectif:
    - Analyser une liste de tokens et créer une liste de commandes.
    - Gérer les redirections, les pipes et les opérateurs logiques.
Retourne:
    - Une liste chaînée de commandes (t_cmd).
Fonctionnement général :
    - Parcourt les tokens un par un.
    - Gère les types suivants :
    - `WORD` : arguments ou commandes simples.
    - `PIPE` : fin d’une commande, enchaîne avec la suivante.
    - `REDIR_*` : redirections (`>`, `<`, `>>`, `<<`) avec vérification que le fichier suit.
    - `(` et `)` : gestion récursive des sous-commandes (sous-shells).
    - `&&` / `||` : opérateurs logiques (conjonction / disjonction).
Détail:
    - Si le token est un `WORD` :
        - Si `current_cmd` est NULL, on la crée.
        - Ajoute le mot comme argument à la commande courante.

    - Si le token est un `PIPE` :
        - Syntax error si pas de commande en cours.
        - Ajoute `current_cmd` à la liste globale, et recommence une nouvelle.

    - Si le token est une redirection :
        - Vérifie que le token suivant est bien un `WORD` (nom du fichier).
        - Associe l'infile, outfile ou heredoc à la commande courante.
        - Avance de deux tokens (opérateur + fichier).

    - Si le token est une parenthèse ouvrante `(` :
        - Appelle récursivement `parse_tokens()` pour parser la sous-commande.
        - Associe le résultat à `current_cmd->left`.
        - Avance les tokens jusqu’à la parenthèse fermante correspondante `)`.

    - Si le token est `AND` ou `OR` :
        - Syntax error si pas de commande à gauche.
        - Crée une nouvelle commande logique (type AND/OR) avec la commande courante en `left`.
        - Avance sur la droite (appel récursif).
        - Associe la commande suivante à `right`.
        - L'information AND/OR est stockée via le champ `append` (ce qui est un peu détourné !).

    - A la fin, si une commande courante est en cours, elle est ajoutée à la liste.
*/

// t_cmd *parse_tokens(t_token *tokens)
// {
//     t_cmd   *cmds;
//     t_cmd   *current_cmd;

//     cmds = NULL;
//     current_cmd = NULL;
//     if (!tokens)
//         return (NULL);
//     while (tokens)
//     {
//         if (tokens->type == WORD)
//         {
//             if (!current_cmd)
//                 current_cmd = new_cmd();
//             if (current_cmd)
//                 add_argument(current_cmd, tokens->value);
//         }
//         else if (tokens->type == PIPE)
//         {
//             if (!current_cmd)
//             {
//                 fprintf(stderr, "Syntax error: unexpected '|'\n");
//                 free_cmds(cmds);
//                 return (NULL);
//             }
//             add_cmd(&cmds, current_cmd);
//             current_cmd = NULL;
//         }
//         else if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT || 
//                  tokens->type == REDIR_APPEND || tokens->type == HEREDOC)
//         {
//             if (!current_cmd)
//                 current_cmd = new_cmd();
//             if (!tokens->next || tokens->next->type != WORD)
//             {
//                 fprintf(stderr, "Syntax error: missing file for redirection\n");
//                 free_cmds(cmds);
//                 if (current_cmd)
//                     free_cmds(current_cmd);
//                 return (NULL);
//             }
//             if (tokens->type == REDIR_IN)
//                 current_cmd->infile = ft_strdup(tokens->next->value);
//             else if (tokens->type == REDIR_OUT)
//             {
//                 current_cmd->outfile = ft_strdup(tokens->next->value);
//                 current_cmd->append = 0;
//             }
//             else if (tokens->type == REDIR_APPEND)
//             {
//                 current_cmd->outfile = ft_strdup(tokens->next->value);
//                 current_cmd->append = 1;
//             }
//             else if (tokens->type == HEREDOC)
//             {
//                 current_cmd->heredoc = 1;
//                 current_cmd->limiter = ft_strdup(tokens->next->value);
//             }
//             tokens = tokens->next;
//         }
//         else if (tokens->type == LPAREN)
//         {
//             tokens = tokens->next; // Passer la parenthèse ouvrante
//             t_cmd *sub_cmd = parse_tokens(tokens); // Appel récursif pour analyser la sous-commande
//             if (!sub_cmd)
//             {
//                 fprintf(stderr, "Syntax error: unmatched '('\n"); // forbidden function
//                 free_cmds(cmds);
//                 return (NULL);
//             }
//             if (!current_cmd)
//                 current_cmd = new_cmd();
//             current_cmd->left = sub_cmd; // Associer la sous-commande
//             while (tokens && tokens->type != RPAREN)
//                 tokens = tokens->next; // Avancer jusqu'à la parenthèse fermante
//             if (!tokens || tokens->type != RPAREN)
//             {
//                 fprintf(stderr, "Syntax error: unmatched '('\n"); // forbidden function
//                 free_cmds(cmds);
//                 return (NULL);
//             }
//         }
//         else if (tokens->type == AND || tokens->type == OR)
//         {
//             if (!current_cmd)
//             {
//                 fprintf(stderr, "Syntax error: unexpected '%s'\n", tokens->value); // forbidden function
//                 free_cmds(cmds);
//                 return (NULL);
//             }
//             t_cmd *new_cmds = new_cmd();
//             new_cmds->left = current_cmd; // Associer la commande courante comme sous-commande gauche
//             new_cmds->append = (tokens->type == AND) ? 1 : 2; // 1 pour AND, 2 pour OR, norminette error
//             current_cmd = new_cmds;
//             tokens = tokens->next; // Passer l'opérateur logique
//             if (tokens->type == LPAREN)
//             {
//                 tokens = tokens->next; // Passer la parenthèse ouvrante
//                 t_cmd *sub_cmd = parse_tokens(tokens); // Appel récursif pour analyser la sous-commande
//                 if (!sub_cmd)
//                 {
//                     fprintf(stderr, "Syntax error: unmatched '('\n"); // forbidden function
//                     free_cmds(cmds);
//                     return (NULL);
//                 }
//                 current_cmd->right = sub_cmd; // Associer la sous-commande droite
//                 while (tokens && tokens->type != RPAREN)
//                     tokens = tokens->next; // Avancer jusqu'à la parenthèse fermante
//                 if (!tokens || tokens->type != RPAREN)
//                 {
//                     fprintf(stderr, "Syntax error: unmatched '('\n");
//                     free_cmds(cmds);
//                     return (NULL);
//                 }
//             }
//             else
//             {
//                 t_cmd *right_cmd = parse_tokens(tokens); // Analyser la commande droite
//                 if (!right_cmd)
//                 {
//                     fprintf(stderr, "Syntax error: missing command after '%s'\n", tokens->value); // forbidden function
//                     free_cmds(cmds);
//                     return (NULL);
//                 }
//                 current_cmd->right = right_cmd;
//             }
//         }
//         tokens = tokens->next;
//     }
//     if (current_cmd)
//         add_cmd(&cmds, current_cmd);
//     return (cmds);
// }

t_token *find_matching_paren(t_token *tokens)
{
    int paren_count = 1;
    t_token *current = tokens;
    
    while (current && paren_count > 0)
    {
        if (current->type == LPAREN)
            paren_count++;
        else if (current->type == RPAREN)
            paren_count--;
        if (paren_count > 0)
            current = current->next;
    }
    return (current);
}

t_token *extract_tokens_between_parens(t_token *start, t_token *end)
{
    t_token *extracted = NULL;
    t_token *current = start;
    
    while (current && current != end)
    {
        t_token *new_tokens = new_token(ft_strdup(current->value), current->type);
        add_token(&extracted, new_tokens);
        current = current->next;
    }
    return (extracted);
}

t_cmd *parse_simple_command(t_token **tokens)
{
    t_cmd *cmd = NULL;
    t_token *current = *tokens;
    
    while (current && current->type != AND && current->type != OR)
    {
        if (current->type == WORD)
        {
            if (!cmd)
                cmd = new_cmd();
            if (cmd)
                add_argument(cmd, current->value);
            current = current->next;
        }
        else if (current->type == PIPE)
        {
            if (!cmd)
            {
                fprintf(stderr, "Syntax error: unexpected '|'\n");
                return (NULL);
            }
            break;
        }
        else if (current->type == REDIR_IN || current->type == REDIR_OUT || 
                 current->type == REDIR_APPEND || current->type == HEREDOC)
        {
            if (!cmd)
                cmd = new_cmd();
            if (!current->next || current->next->type != WORD)
            {
                fprintf(stderr, "Syntax error: missing file for redirection\n");
                if (cmd)
                    free_cmds(cmd);
                return (NULL);
            }
            
            if (current->type == REDIR_IN)
                cmd->infile = ft_strdup(current->next->value);
            else if (current->type == REDIR_OUT)
            {
                cmd->outfile = ft_strdup(current->next->value);
                cmd->append = 0;
            }
            else if (current->type == REDIR_APPEND)
            {
                cmd->outfile = ft_strdup(current->next->value);
                cmd->append = 1;
            }
            else if (current->type == HEREDOC)
            {
                cmd->heredoc = 1;
                cmd->limiter = ft_strdup(current->next->value);
            }
            current = current->next->next;
        }
        else if (current->type == LPAREN)
        {
            t_token *closing_paren = find_matching_paren(current->next);
            if (!closing_paren)
            {
                fprintf(stderr, "Syntax error: unmatched '('\n");
                if (cmd)
                    free_cmds(cmd);
                return (NULL);
            }
            
            t_token *sub_tokens = extract_tokens_between_parens(current->next, closing_paren);
            if (sub_tokens)
            {
                t_cmd *sub_cmd = parse_tokens(sub_tokens);
                if (!sub_cmd)
                {
                    fprintf(stderr, "Syntax error in subcommand\n");
                    free_tokens(sub_tokens);
                    if (cmd)
                        free_cmds(cmd);
                    return (NULL);
                }
                if (!cmd)
                {
                    // Cas simple: (command) - on retourne directement la sous-commande
                    cmd = sub_cmd;
                }
                else
                {
                    // Cas complexe: on associe à la commande courante
                    cmd->left = sub_cmd;
                }
                free_tokens(sub_tokens);
            }
            current = closing_paren->next;
        }
        else if (current->type == RPAREN)
        {
            break;
        }
        else
        {
            current = current->next;
        }
    }
    *tokens = current;
    return (cmd);
}

t_cmd *parse_pipeline(t_token **tokens)
{
    t_cmd *cmds = NULL;
    t_cmd *left_cmd = parse_simple_command(tokens);
    
    if (!left_cmd)
        return (NULL);
    
    add_cmd(&cmds, left_cmd);
    
    while (*tokens && (*tokens)->type == PIPE)
    {
        *tokens = (*tokens)->next;
        t_cmd *right_cmd = parse_simple_command(tokens);
        if (!right_cmd)
        {
            fprintf(stderr, "Syntax error: expected command after '|'\n");
            free_cmds(cmds);
            return (NULL);
        }
        add_cmd(&cmds, right_cmd);
    }
    return (cmds);
}

t_cmd *parse_tokens(t_token *tokens)
{
    if (!tokens)
        return (NULL);
    t_token *current = tokens;
    t_cmd *left_cmd = parse_pipeline(&current);
    
    if (!left_cmd)
        return (NULL);
    if (current && (current->type == AND || current->type == OR))
    {
        t_cmd *logical_cmd = new_cmd();
        if (!logical_cmd)
        {
            free_cmds(left_cmd);
            return (NULL);
        }
        
        logical_cmd->left = left_cmd;
        logical_cmd->append = (current->type == AND) ? 1 : 2; // 1 pour AND, 2 pour OR, norme error
        current = current->next;
        if (!current)
        {
            fprintf(stderr, "Syntax error: missing command after logical operator\n"); // forbidden fonction
            free_cmds(logical_cmd);
            return (NULL);
        }
        t_cmd *right_cmd = parse_tokens(current); // Récursion pour la partie droite
        if (!right_cmd)
        {
            fprintf(stderr, "Syntax error: invalid right side of logical operator\n"); // forbidden fonction
            free_cmds(logical_cmd);
            return (NULL);
        }
        logical_cmd->right = right_cmd;
        return (logical_cmd);
    }
    return (left_cmd);
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