/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:13:03 by niclee            #+#    #+#             */
/*   Updated: 2025/08/21 15:13:32 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_match(const char *str, const char *pattern)
{
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern == '*')
		return (ft_match(str, pattern + 1) || (*str && ft_match(str + 1,
					pattern)));
	if (*str == *pattern)
		return (ft_match(str + 1, pattern + 1));
	return (0);
}

static char	*append_char_to_result(char *result, char c)
{
	char	*tmp;
	char	char_str[2];

	char_str[0] = c;
	char_str[1] = '\0';
	tmp = ft_strjoin(result, char_str);
	free(result);
	return (tmp);
}

static char	*append_var_value(char *result, char *val)
{
	char	*tmp;

	tmp = ft_strjoin(result, val);
	free(result);
	free(val);
	return (tmp);
}

static char	*process_variable(const char **p, char **env, char *result)
{
	char							*code;
	const char						*start;
	char							*var;
	char							*val;
	extern volatile sig_atomic_t	g_sig_value;

	(*p)++;
	if (**p == '?')
	{
		code = ft_itoa(g_sig_value);
		result = append_var_value(result, code);
		(*p)++;
		return (result);
	}
	start = *p;
	while (ft_isalnum(**p) || **p == '_')
		(*p)++;
	var = ft_substr(start, 0, *p - start);
	val = expand_env(var, env);
	free(var);
	if (val)
		result = append_var_value(result, val);
	return (result);
}

char	*expand_all_vars(const char *str, char **env)
{
	char		*result;
	const char	*p;

	result = ft_calloc(1, 1);
	p = str;
	while (*p)
	{
		if (*p == '$' && (ft_isalnum(*(p + 1)) || *(p + 1) == '_' || *(p
					+ 1) == '?'))
			result = process_variable(&p, env, result);
		else
		{
			result = append_char_to_result(result, *p);
			p++;
		}
	}
	return (result);
}
