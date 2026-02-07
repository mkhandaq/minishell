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
	if(!check_syntax_errors(tmp))
		return ;
	while(tmp->next)
	{
		if(is_one_sided(tmp))
			tmp->next->type = TOK_KEYWORD;
		else if(is_two_sided(tmp))
			tmp->next->type = TOK_CMD;
		tmp = tmp->next;
	}
	tmp = *list;
	while (tmp->next)
	{
		if(tmp->type == TOK_CMD && !(is_sign(tmp->next)))
			tmp->next->type = TOK_KEYWORD;
		tmp = tmp->next;
	}
}
