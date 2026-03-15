/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tree_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:18:54 by aalemami          #+#    #+#             */
/*   Updated: 2026/03/15 09:18:54 by aalemami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_last_op_tok(t_token *cur, int prec)
{
	if (prec == 0 && (cur->type == TOK_AND || cur->type == TOK_OR))
		return (1);
	return (prec == 1 && cur->type == TOK_PIPE);
}

static void	update_last_op(t_token *cur, t_token *prv, t_token **last)
{
	last[0] = cur;
	last[1] = prv;
}

t_token	*find_last_op(t_token *tokens, int prec, t_token **prev)
{
	t_token	*last[2];
	t_token	*prv;
	t_token	*cur;
	int		depth;

	last[0] = NULL;
	last[1] = NULL;
	prv = NULL;
	cur = tokens;
	depth = 0;
	while (cur)
	{
		if (cur->type == TOK_OPENBRC)
			depth++;
		else if (cur->type == TOK_CLOSEBRC)
			depth--;
		else if (!depth && is_last_op_tok(cur, prec))
			update_last_op(cur, prv, last);
		prv = cur;
		cur = cur->next;
	}
	*prev = last[1];
	return (last[0]);
}

int	is_wrapped_in_parens(t_token *tokens)
{
	t_token	*cur;
	int		depth;

	if (!tokens || tokens->type != TOK_OPENBRC)
		return (0);
	depth = 0;
	cur = tokens;
	while (cur)
	{
		if (cur->type == TOK_OPENBRC)
			depth++;
		else if (cur->type == TOK_CLOSEBRC)
		{
			depth--;
			if (depth == 0 && cur->next != NULL)
				return (0);
		}
		cur = cur->next;
	}
	return (depth == 0);
}
