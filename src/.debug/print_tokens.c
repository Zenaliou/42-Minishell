#include "../includes/minishell.h"

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("TOKEN type = %d, value = \"%s\"\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
}
