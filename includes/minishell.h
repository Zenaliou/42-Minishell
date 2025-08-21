/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gule-bat <gule-bat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:10:38 by niclee            #+#    #+#             */
/*   Updated: 2025/08/21 18:26:17 by gule-bat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "get_next_line.h"
# include "libft.h"
# include <ctype.h>
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdatomic.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

// # define PROMPT "minishell> "
# define PROMPT "\e[45m \e[41m\e[4;32m[ minishell ]\e[0m -> "
# define PROMPT2 "minishell\n"
# define MAXASCII 257

// typedef struct s_token {
extern volatile sig_atomic_t	g_sig_value;

typedef enum e_token_type
{
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
}								t_token_type;

typedef enum e_quot_state
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}								t_quote_state;

typedef struct s_token
{
	char						*value;
	t_token_type				type;
	t_quote_state				quote_state;
	struct s_token				*next;
}								t_token;

typedef struct s_cmd
{
	char						**argv;
	char						*infile;
	char						*outfile;
	int							append;
	int							heredoc;
	char						*limiter;
	int							hd_fd;
	int							operator;
	int							err;
	struct s_cmd				*left;
	struct s_cmd				*right;
	struct s_cmd				*next;
	pid_t						pid;
}								t_cmd;

typedef struct s_envi
{
	char						*full;
	struct s_envi				*next;
}								t_env;

typedef struct s_shell
{
	t_env						*env;
	char						**envtab;
	t_cmd						*cmd;
	t_cmd						*head;
	char						*line;
	int							*in;
	int							*out;
}								t_shell;

typedef struct s_stock
{
	t_env						*env;
	t_cmd						*cmd_head;
	t_cmd						*curr_cmd;
	t_token						*token_head;
	t_token						*curr_token;
}								t_stock;

// Tokenizing
t_token							*tokenize(char *input);
char							*handle_symbol(char *current, t_token **tokens,
									t_quote_state *qs);
char							*handle_pipe_and_or(char *current,
									t_token **tokens);
char							*handle_parens(char *current, t_token **tokens);
char							*handle_word(char *current, t_token **tokens,
									t_quote_state *qs);
t_token							*handle_unclosed_quote(t_token *tokens);
char							*handle_redirect_out(char *current,
									t_token **tokens);
char							*handle_redirect_in(char *current,
									t_token **tokens);
t_token							*new_token(char *value, t_token_type type);
void							add_token(t_token **list, t_token *new);
char							*extract_word(char **input);

// Parsing
t_cmd							*parse_tokens(t_token *tokens, t_env *env);
t_cmd							*parse_simple_command(t_token **tokens,
									t_stock *stock);
t_token							*handle_redirection(t_cmd **cmd, t_token *token,
									t_stock *stock);
t_token							*handle_subshell(t_cmd **cmd, t_token *token,
									t_env *env);
t_token							*find_matching_paren(t_token *tokens);
t_token							*extract_tokens_between_parens(t_token *start,
									t_token *end);

// CMD
void							add_argument(t_cmd *cmd, char *arg);
t_cmd							*new_cmd(void);
void							add_cmd(t_cmd **cmds, t_cmd *new_cmd);
char							*extract_quoted_word(char **input,
									t_quote_state *state);
// Expansion
void							expand_variables(t_token *tokens, char **env);
char							*expand_env(char *str, char **env);
void							expand_wildcards(t_token *tokens);
int								ft_match(const char *str, const char *pattern);
char							*expand_all_vars(const char *str, char **env);

// Memory
void							free_tokens(t_token *tokens);
void							free_cmds(t_cmd *cmds);
void							freetab(char **str);
void							free_env(t_env *env);
void							free_sub_proc(char *path, t_env *envi,
									t_cmd *tmp, t_shell *shell);

// Execution (à compléter selon ton projet)
int								exec_handler(t_cmd *cmds, char **env,
									t_env **envi);
int								process_board(t_shell *shell);
int								piping(t_shell **shell, int *fd);


// BUILT_IN
int								builtin_echo(char **args);
void							builtin_pwd(void);
void							builtin_cd(char **argv, t_env *envi);
void							builtin_env(t_shell *shell);
void							builtin_unset(t_shell **shell);
int								builtin_export(t_shell **shell);
int								builtin_exit(char **args, t_shell **shell);
int								is_builtin(char *str);
void							builtin_finder(t_shell **shell, int i);
int								is_bt_int(int i);
char							*go_to_equal(char *str);

// fake env
int								nodeend(t_env **head, char *env);
int								nodenewhead(t_env **head, char *envline);
int								fakeenv(char **env, t_env **envi);
int								search_env(char *line, char *baseline);
char							**getenvmini(t_env *env);
char							**list_to_tab(t_env *env);
void	buff_handling(char *buffer, int val, int i, t_env *tmp);
// DEBUG
void							print_cmds(t_cmd *cmds);
void							print_tokens(t_token *tokens);
int								checkerror(t_cmd *cmds);

// SIGNALS
void							sig_handler(void);
void							sig_handler_begin(void);
void							sig_dealer(int signum, siginfo_t *info,
									void *context);
void							sig_dealer_begin(int signum, siginfo_t *info,
									void *context);

// STRING HANDLING EXEC
char							*ft_str_n_join(char *str, char *n, char *join);
char							*pathchecker(char *cmd, t_env *env);
char							*pathing(t_cmd *cmds, t_env *env);

// HEREDOC
int								dochere(int *fd, t_token *limiter, t_env *env);
int								heredoc(t_stock *stock);
void							print_heredoc(t_cmd *cmd, char *infile,
									int *fd);
void							dupclose(int *fd, int entry);

// REDIR
void							redir_in(t_shell **shell, int *fd);
void							redir_out(t_shell **shell, int *fd);
t_shell							shell_init(t_cmd *cmds, char **env,
									t_env **envi);
void							redirs(t_shell **shell, int *p_fd, int *s_fd);
pid_t							waiting_and_status(pid_t *pid, int *i);
void							dupclose(int *fd, int entry);

#endif