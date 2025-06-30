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
# include <sys/wait.h>
# include <limits.h>

# define prompt "\e[45m \e[41m\e[4;32m~\t <}-)[minishell](-{> \t~\e[45m \e[0m\n~> \e[1;35m$\e[0m "
# define prompt2 "\e[45m \e[41m\e[4;32m~\t <}-)[minishell](-{> \t~\e[45m \e[0m\n"

// typedef struct s_token {
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
	pid_t pid;
} t_cmd;

typedef struct s_envi {
	char			*full;
	struct s_envi	*next;
} t_env;

typedef struct s_shell
{
	t_env *env;
	char **envtab;
	t_cmd *cmd;
	t_cmd *head;
	char *line;
} t_shell;

// Tokenizing
t_token *tokenize(char *input);
char	*handle_symbol(char *current, t_token **tokens, t_quote_state *qs);
char	*handle_pipe_and_or(char *current, t_token **tokens);
char	*handle_parens(char *current, t_token **tokens);
char	*handle_word(char *current, t_token **tokens, t_quote_state *qs);
t_token	*handle_unclosed_quote(t_token *tokens);
char	*handle_redirect_out(char *current, t_token **tokens);
char	*handle_redirect_in(char *current, t_token **tokens);
t_token	*new_token(char *value, t_token_type type);
void    add_token(t_token **list, t_token *new);
char    *extract_word(char **input);

// Parsing
t_cmd   *parse_tokens(t_token *tokens);
t_cmd	*parse_simple_command(t_token **tokens);
t_token	*handle_redirection(t_cmd **cmd, t_token *token);
t_token	*handle_subshell(t_cmd **cmd, t_token *token);
t_token	*find_matching_paren(t_token *tokens);
t_token	*extract_tokens_between_parens(t_token *start, t_token *end);

// CMD
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


// Memory
void    free_tokens(t_token *tokens);
void    free_cmds(t_cmd *cmds);
void    freetab(char **str);
void	free_env(t_env *env);

// Execution (à compléter selon ton projet)
int exec_handler(t_cmd *cmds, char **env, t_env *envi);
// int	process_board(t_cmd *cmds, t_env *envi, char **env);
int	process_board(t_shell *shell);

	// BUILT_IN
int	    builtin_echo(char **args);
void	builtin_pwd(void);
void	builtin_cd(char **argv, t_env *envi);
void	builtin_env(t_env *envi);
void	builtin_unset(char **argv, t_env **envi); // BANCAL DE FOU
int	    builtin_exit(char **args);

// fake env
int	nodeend(t_env **head, char	*env);
int nodenewhead(t_env **head, char *envline);
int	fakeenv(char **env, t_env **envi);
// DEBUG
void    print_cmds(t_cmd *cmds);
void    print_tokens(t_token *tokens);

// SIGNALS
void    sig_handler(void);
void    sig_dealer(int  signum, siginfo_t *info, void *context); //gives the value of the signal to the global variable to catch them


#endif