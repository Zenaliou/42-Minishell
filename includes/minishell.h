#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <ctype.h>
# include "libft.h"
# include <dirent.h>
# include <signal.h>
# include <stdatomic.h>

# define prompt "\e[45m \e[41m\e[4;32m~\t <}-)[minishell](-{> \t~\e[45m \e[0m\n~> \e[1;35m$\e[0m "
# define prompt2 "\e[45m \e[41m\e[4;32m~\t <}-)[minishell](-{> \t~\e[45m \e[0m\n"

extern volatile sig_atomic_t sig_value; // Pour gérer les signaux ctrl + c, ctrl+D et ctrl+\ /


typedef enum e_token_type {
    WORD,
    PIPE, 
    REDIR_IN, 
    REDIR_OUT,
    REDIR_APPEND,
    HEREDOC,
    AND,
    OR,
    LPAREN,
    RPAREN
} t_token_type;

typedef enum e_quot_state
{
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
}   t_quote_state;

typedef struct s_token {
    char *value;
    t_token_type type;
    struct s_token *next;
} t_token;

typedef struct s_cmd {
    char **argv;
    char *infile;
    char *outfile;
    int append;
    int heredoc;
    char *limiter;
    int operator;
    struct s_cmd *left;
    struct s_cmd *right;
    struct s_cmd *next;
} t_cmd;

// Tokenizing
t_token	*new_token(char *value, t_token_type type);
t_token *tokenize(char *input);
void    add_token(t_token **list, t_token *new);
char    *extract_word(char **input);

// Parsing
t_cmd   *parse_tokens(t_token *tokens);
void    add_argument(t_cmd *cmd, char *arg);
t_cmd   *new_cmd(void);
void    add_cmd(t_cmd **cmds, t_cmd *new_cmd);
char	*extract_quoted_word(char **input, t_quote_state *state);

// Expansion
void    expand_variables(t_token *tokens, char **env);
char    *expand_env(char *str, char **env);
void	expand_wildcards(t_token *tokens);
int	    ft_match(const char *str, const char *pattern);
char    *expand_all_vars(const char *str, char **env);

// Execution (à compléter selon ton projet)
void    execute_commands(t_cmd *cmds, char **env);

// Memory
void    free_tokens(t_token *tokens);
void    free_cmds(t_cmd *cmds);

// DEBUG
void print_cmds(t_cmd *cmds);


// SIGNALS
void    sig_handler(void);
// void    sig_dealer(int  signum, siginfo_t *info, void *context); //gives the value of the signal to the global variable to catch them


#endif