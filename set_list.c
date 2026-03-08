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

static void	push(t_token **list, char *value, t_strtype stype)
{
	t_token	*new;
	t_token	*tmp;

	tmp = *list;
	if(tmp && tmp->next)
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

static char	*read_token(char *input, int *i, t_strtype *stype)
{
	char	*result;
	int		start;
	char	q;

	start = *i;
	if (!input)
		return (NULL);
	while (input[*i] && input[*i] != ' ')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			q = input[(*i)++];
			while (input[*i] && input[*i] != q)
				(*i)++;
			if (!input[*i])
				break ;
		}
		(*i)++;
	}
	*stype = TOK_STR;
	if (input[start] == '\'')
		*stype = TOK_SING;
	else if (input[start] == '"')
		*stype = TOK_DUP;
	if (*stype != TOK_STR && (*i - start) >= 2)
		result = ft_substr(input, start + 1, *i - start - 2);
	else
		result = ft_substr(input, start, *i - start);
	return (result);
}

t_token	*set_list(char *input)
{
	t_token		*list;
	char		*token;
	int			i;
	t_strtype	stype;

	list = NULL;
	i = 0;
	if(!input)
		return (NULL);
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (!input[i])
			break ;
		token = read_token(input, &i, &stype);
		if (token && token[0])
			push(&list, token, stype);
		free(token);
	}
	return (list);
}