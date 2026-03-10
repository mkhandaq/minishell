/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 08:32:16 by aalemami          #+#    #+#             */
/*   Updated: 2026/03/01 08:32:16 by aalemami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tree	*new_tree_node(t_toktype type)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->type = type;
	node->tokens = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static t_token	*strip_parens(t_token *tokens)
{
	t_token	*inner;
	t_token	*cur;
	t_token	*prev;

	inner = tokens->next;
	prev = NULL;
	cur = inner;
	while (cur && cur->next)
	{
		prev = cur;
		cur = cur->next;
	}
	if (prev)
		prev->next = NULL;
	else
		inner = NULL;
	free(tokens->value);
	free(tokens);
	if (cur && cur->type == TOK_CLOSEBRC)
	{
		free(cur->value);
		free(cur);
	}
	return (inner);
}

static t_tree	*split_at_op_right(t_tree *node, t_token *tokens,
		t_token *prev, t_token *right)
{
	node->right = build_tree(right);
	if (!node->right)
	{
		if (prev)
			free_list(&tokens);
		return (free_tree(node), NULL);
	}
	return (node);
}

static t_tree	*split_at_op(t_token *tokens, t_token *op, t_token *prev)
{
	t_tree	*node;
	t_token	*right_tokens;

	node = new_tree_node(op->type);
	if (!node)
		return (free_list(&tokens), NULL);
	node->tokens = op;
	if (prev)
		prev->next = NULL;
	right_tokens = op->next;
	op->next = NULL;
	if (right_tokens && !split_at_op_right(node, tokens, prev, right_tokens))
		return (NULL);
	if (prev)
	{
		node->left = build_tree(tokens);
		if (!node->left)
			return (free_tree(node), NULL);
	}
	return (node);
}

t_tree	*build_tree(t_token *tokens)
{
	t_tree	*node;
	t_token	*op;
	t_token	*prev;

	if (!tokens)
		return (NULL);
	if (is_wrapped_in_parens(tokens))
	{
		node = new_tree_node(TOK_SUBSHELL);
		if (!node)
			return (free_list(&tokens), NULL);
		node->left = build_tree(strip_parens(tokens));
		return (node);
	}
	op = find_last_op(tokens, 0, &prev);
	if (!op)
		op = find_last_op(tokens, 1, &prev);
	if (op)
		return (split_at_op(tokens, op, prev));
	node = new_tree_node(TOK_CMD);
	if (!node)
		return (free_list(&tokens), NULL);
	node->tokens = tokens;
	return (node);
}
