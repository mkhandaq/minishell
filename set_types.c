/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_types.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 02:31:08 by marvin            #+#    #+#             */
/*   Updated: 2026/02/07 02:31:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	split_sign_node(t_token **list)
{
	t_token *tmp;
	t_token *tmp_ptr;
	t_token *new;

	tmp = *list;
	tmp_ptr = NULL;
	if(is_sign(tmp))
	{
		new = malloc(sizeof(t_token));
		if (!new)
			return ;
		if(return_sign_len(tmp) == 1 && ft_strlen(tmp->value) > 1)
			new->value = ft_strdup(tmp->value + 1);
		else if(return_sign_len(tmp) == 2 && ft_strlen(tmp->value) > 2)
			new->value = ft_strdup(tmp->value + 2);
		else
		{
			free(new);
			return ;
		}
		new->type = TOK_KEYWORD;
		if(tmp->next)
			tmp_ptr = tmp->next;
		tmp->next = new;
		new->next = tmp_ptr;
	}
}

static void	split_sign_node_loop(t_token **list)
{
	t_token *tmp;

	tmp = *list;
	while(tmp)
	{
		split_sign_node(&tmp);
		tmp = tmp->next;
	}
}


void	set_types(t_token **list)
{
	t_token *tmp;

	tmp = *list;
	while(tmp)
	{
		set_sign(&tmp);
		tmp = tmp->next;
	}
	tmp = *list;
	split_sign_node_loop(&tmp);
	if(!check_syntax_errors(tmp))
		return ;
	while(tmp && tmp->next)
	{
		if(is_one_sided(tmp))
			tmp->next->type = TOK_KEYWORD;
		else if(is_two_sided(tmp))
			tmp->next->type = TOK_CMD;
		tmp = tmp->next;
	}
	tmp = *list;
	while (tmp && tmp->next)
	{
		if(tmp->type == TOK_CMD && !(is_sign(tmp->next)))
			tmp->next->type = TOK_KEYWORD;
		tmp = tmp->next;
	}
}
