/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:19:13 by aalemami          #+#    #+#             */
/*   Updated: 2026/03/15 09:19:14 by aalemami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_charjoin(char *s, char c)
{
	char	*result;
	int		len;

	if (!s)
	{
		result = malloc(2);
		if (!result)
			return (NULL);
		result[0] = c;
		result[1] = '\0';
		return (result);
	}
	len = ft_strlen(s);
	result = malloc(len + 2);
	if (!result)
		return (free(s), NULL);
	ft_memcpy(result, s, len);
	result[len] = c;
	result[len + 1] = '\0';
	free(s);
	return (result);
}

static char	*lookup_var(const char *s, int *i, char **env)
{
	char	*var_name;
	char	*value;
	int		start;

	start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	var_name = ft_substr(s, start, *i - start);
	if (!var_name)
		return (NULL);
	value = ft_getenv(env, var_name);
	free(var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static char	*expand_dollar(const char *s, int *i, int exit_status,
		char **env)
{
	(*i)++;
	if (s[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(exit_status));
	}
	if (ft_isdigit(s[*i]))
	{
		(*i)++;
		return (ft_strdup(""));
	}
	if (!ft_isalpha(s[*i]) && s[*i] != '_')
		return (ft_strdup("$"));
	return (lookup_var(s, i, env));
}

char	*expand_str(const char *s, int exit_status, char **env)
{
	char	*result;
	char	*tmp;
	char	*old;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && (s[i + 1] == '?'
				|| ft_isalpha(s[i + 1]) || s[i + 1] == '_'
				|| ft_isdigit(s[i + 1])))
		{
			tmp = expand_dollar(s, &i, exit_status, env);
			old = result;
			result = ft_strjoin(result, tmp);
			free(old);
			free(tmp);
		}
		else
			result = ft_charjoin(result, s[i++]);
	}
	return (result);
}

void	expand_tokens(t_token *list, int exit_status, char **env)
{
	char	*expanded;

	while (list)
	{
		if (list->strtype != TOK_SING && list->type != TOK_LIMITER
			&& list->value && ft_strchr(list->value, '$'))
		{
			expanded = expand_str(list->value, exit_status, env);
			free(list->value);
			list->value = expanded;
		}
		list = list->next;
	}
}
