/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_qoutes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 15:46:00 by marvin            #+#    #+#             */
/*   Updated: 2026/02/19 15:46:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	push(t_token **list, char *value)
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
	new->type = 0;
	new->is_exuted = 0;
	new->next = NULL;
	if (tmp)
		tmp->next = new;
	else
		*list = new;
}

static char	*read_token(char *input, int *i)
{
	char	*result;
	int		start;
	int		len;

	start = *i;
	if(!input)
		return (NULL);
	while (input[*i] && input[*i] != ' ')
	{
		if (input[*i] == '\'')
		{
			(*i)++;
			while (input[*i] && input[*i] != '\'')
				(*i)++;
		}
		else if (input[*i] == '"')
		{
			(*i)++;
			while (input[*i] && input[*i] != '"')
				(*i)++;
		}
		(*i)++;
	}
	len = *i - start;
	if ((input[start] == '\'' || input[start] == '"') && len >= 2)
		result = ft_substr(input, start + 1, len - 2);
	else
		result = ft_substr(input, start, len);
	return (result);
}


t_token	*set_list(char *input)
{
	t_token	*list;
	char	*token;
	int		i;

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
		token = read_token(input, &i);
		if (token && token[0])
			push(&list, token);
		free(token);
	}
	return (list);
}