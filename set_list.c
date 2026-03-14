/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:08:30 by ali_shell         #+#    #+#             */
/*   Updated: 2026/03/08 16:08:30 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*remove_quotes(char *str)
{
	char	*new_str;
	int		i;
	int		j;
	int		sing;
	int		dup;

	if (!str)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
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
	free(str);
	return (new_str);
}

static void	push(t_token **list, char *value, t_strtype stype)
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

static t_strtype	get_stype(char *input, int start)
{
	if (input[start] == '\'')
		return (TOK_SING);
	if (input[start] == '"')
		return (TOK_DUP);
	return (TOK_STR);
}

static char	*read_token(char *input, int *i, t_strtype *stype)
{
	int		start;
	char	q;

	start = *i;
	if (!input)
		return (NULL);
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
			continue ;
		}
		(*i)++;
	}
	*stype = get_stype(input, start);
	return (ft_substr(input, start, *i - start));
}

t_token	*set_list(char *input)
{
	t_token		*list;
	char		*token;
	int			i;
	t_strtype	stype;

	list = NULL;
	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		while (input[i] && (input[i] == ' ' || input[i] == '\t'))
			i++;
		if (!input[i])
			break ;
		token = read_token(input, &i, &stype);
		if (token)
			token = remove_quotes(token);
		if (token && token[0])
			push(&list, token, stype);
		free(token);
	}
	return (list);
}

void	split_node_loop(t_token **list)
{
	t_token	*tmp;

	tmp = *list;
	while (tmp)
	{
		if (split_nonsign_node(&tmp))
			tmp = tmp->next;
		split_sign_node(&tmp);
		tmp = tmp->next;
	}
}
