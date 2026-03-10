/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brackets.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 16:09:04 by marvin            #+#    #+#             */
/*   Updated: 2026/03/10 16:09:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_closed(t_token *node)
{
	t_token	*tmp;
	int		count_open;

	tmp = node;
	count_open = 0;
	while (tmp && count_open >= 0)
	{
		if (tmp->type == TOK_OPENBRC)
			count_open++;
		if (tmp->type == TOK_CLOSEBRC)
			count_open--;
		tmp = tmp->next;
	}
	if (count_open)
		return (0);
	return (1);
}

static int	check_open_brc(t_token *tmp)
{
	if (tmp->type == TOK_OPENBRC && is_two_sided(tmp->next))
	{
		ft_printf("shellGuys: parse error near `%s'\n", tmp->next->value);
		return (0);
	}
	if (tmp->type == TOK_OPENBRC && tmp->next->type == TOK_CLOSEBRC)
	{
		ft_printf("shellGuys: EMPTY BRACKET\n");
		return (0);
	}
	return (1);
}

static int	check_close_brc(t_token *tmp)
{
	if (tmp->type == TOK_CLOSEBRC && tmp->next->type == TOK_OPENBRC)
	{
		ft_printf("shellGuys: parse error near `('\n");
		return (0);
	}
	if (tmp->type == TOK_CLOSEBRC
		&& (tmp->next->type == TOK_CMD || tmp->next->type == TOK_KEYWORD))
	{
		ft_printf("shellGuys: parse error near `%s'\n", tmp->next->value);
		return (0);
	}
	return (1);
}

static int	check_cmd_brc(t_token *tmp)
{
	if ((tmp->type == TOK_CMD || tmp->type == TOK_KEYWORD)
		&& tmp->next->type == TOK_OPENBRC)
	{
		ft_printf("shellGuys: number expected\n");
		return (0);
	}
	return (1);
}

int	check_brackets(t_token *node)
{
	t_token	*tmp;

	tmp = node;
	if (tmp && !is_closed(tmp))
	{
		ft_printf("shellGuys: CLOSE THAT BRACKET\n");
		return (0);
	}
	while (tmp && tmp->next)
	{
		if (!check_open_brc(tmp) || !check_close_brc(tmp)
			|| !check_cmd_brc(tmp))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}
