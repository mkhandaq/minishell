/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:47:52 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/02/07 20:40:52 by mkhandaq         ###   ########.fr       */
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
	new->value = value;
	new->type = 0;
	if (tmp)
		tmp->next = new;
	else
		*list = new;
}

void	set_list(t_token **list, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		push(list, argv[i]);
        i++;
	}
}
