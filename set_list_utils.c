/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:17:27 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:17:28 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	rmv_quotes_helper(char *str, char *new_str)
{
	int		i;
	int		j;
	int		sing;
	int		dup;

	i = 0;
	j = 0;
	sing = 0;
	dup = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !dup)
			sing = !sing;
		else if (str[i] == '"' && !sing)
			dup = !dup;
		else
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
}

char	*remove_quotes(char *str)
{
	char	*new_str;

	if (!str)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	rmv_quotes_helper(str, new_str);
	free(str);
	return (new_str);
}

void	push(t_token **list, char *value, t_strtype stype)
{
	t_token	*new;
	t_token	*tmp;

	tmp = *list;
	if (tmp && tmp->next)
		while (tmp->next)
			tmp = tmp->next;
	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(value);
	new->type = TOK_KEYWORD;
	new->strtype = stype;
	new->is_exuted = 0;
	new->next = NULL;
	if (tmp)
		tmp->next = new;
	else
		*list = new;
}

static void	read_word_token(char *input, int *i)
{
	char	q;

	while (input[*i] && input[*i] != ' ' && input[*i] != '\t')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			q = input[(*i)++];
			while (input[*i] && input[*i] != q)
				(*i)++;
			if (!input[*i])
				break ;
			(*i)++;
		}
		else if (ft_strchr("|&<>()", input[*i]))
			break ;
		else
			(*i)++;
	}
}

char	*read_token(char *input, int *i, t_strtype *stype)
{
	int		start;

	start = *i;
	if (!input)
		return (NULL);
	if (ft_strchr("|&<>()", input[*i]))
	{
		if ((input[*i] == '|' && input[*i + 1] == '|')
			|| (input[*i] == '&' && input[*i + 1] == '&')
			|| (input[*i] == '<' && input[*i + 1] == '<')
			|| (input[*i] == '>' && input[*i + 1] == '>'))
			(*i) += 2;
		else
			(*i)++;
		*stype = TOK_STR;
		return (ft_substr(input, start, *i - start));
	}
	read_word_token(input, i);
	*stype = TOK_STR;
	if (input[start] == '\'')
		*stype = TOK_SING;
	if (input[start] == '"')
		*stype = TOK_DUP;
	return (ft_substr(input, start, *i - start));
}
