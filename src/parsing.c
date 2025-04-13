#include "../includes/minishell.h"

t_cmd *parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current_cmd;

	cmds = NULL;
	current_cmd = NULL;
	while (tokens)
	{
		if (tokens->type == WORD)
		{
			if (!current_cmd)
				current_cmd = new_cmd();
			add_argument(current_cmd, tokens->value);
		}
		else if (tokens->type == PIPE)
		{
			if (!current_cmd)
			{
				fprintf(stderr, "Syntax error : expected '|'\n"); // forbidden function fprintf
				free_cmds(cmds);
				return (NULL);
			}
			add_cmd(&cmds, current_cmd);
			current_cmd = NULL;
		}
	}
}
