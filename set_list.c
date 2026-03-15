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
