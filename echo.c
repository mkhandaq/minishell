/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 14:55:26 by marvin            #+#    #+#             */
/*   Updated: 2026/03/10 14:55:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	only_n_chars(const char *s)
{
	if (!s || !*s)
		return (0);
	while (*s)
	{
		if (*s != 'n')
			return (0);
		s++;
	}
	return (1);
}

int	echo(t_token **list)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while ((*list)->next && (*list)->next->value[0] == '-'
		&& only_n_chars((*list)->next->value + 1))
	{
		*list = (*list)->next;
		i = 1;
	}
	*list = (*list)->next;
	while (*list && !(is_sign(*list)))
	{
		if (ft_printf("%s", (*list)->value) == -1)
			ret = 1;
		if ((*list)->next && !is_sign((*list)->next))
			if (ft_printf(" ") == -1)
				ret = 1;
		*list = (*list)->next;
	}
	if (!i)
		if (ft_printf("\n") == -1)
			ret = 1;
	return (ret);
}
