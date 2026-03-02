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

static t_token	*find_last_op(t_token *tokens, int prec, t_token **prev)
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
		else if (depth == 0 && ((prec == 0
					&& (cur->type == TOK_AND || cur->type == TOK_OR))
				|| (prec == 1 && cur->type == TOK_PIPE)))
		{
			last[0] = cur;
			last[1] = prv;
		}
		prv = cur;
		cur = cur->next;
	}
	*prev = last[1];
	return (last[0]);
}


static int	is_wrapped_in_parens(t_token *tokens)
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
//if depth is not 0 there is an error 

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

static void	free_token_list(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

static t_tree	*split_at_op(t_token *tokens, t_token *op, t_token *prev)
{
	t_tree	*node;
	t_token	*right_tokens;

	node = new_tree_node(op->type);
	if (!node)
		return (free_token_list(tokens), NULL);
	node->tokens = op;
	if (prev)
		prev->next = NULL;
	right_tokens = op->next;
	op->next = NULL;
	if (right_tokens)
	{
		node->right = build_tree(right_tokens);
		if (!node->right)
		{
			if (prev)
				free_token_list(tokens);
			return (free_tree(node), NULL);
		}
	}
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
		return (build_tree(strip_parens(tokens)));
	op = find_last_op(tokens, 0, &prev);
	if (!op)
		op = find_last_op(tokens, 1, &prev);
	if (op)
		return (split_at_op(tokens, op, prev));
	node = new_tree_node(TOK_CMD);
	if (!node)
		return (free_token_list(tokens), NULL);
	node->tokens = tokens;
	return (node);
}

static const char	*toktype_str(t_toktype type)
{
	const char	*names[] = {
		"CMD", "KEYWORD", "PIPE", "AND", "OR",
		"REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "HEREDOC",
		"INFILE", "OUTFILE", "LIMITER",
		"OPENBRC", "CLOSEBRC",
		"ECHO", "CD", "PWD", "EXPORT", "UNSET", "ENV", "EXIT"
	};

	if (type >= 0 && type <= TOK_EXIT)
		return (names[type]);
	return ("UNKNOWN");
}

static void	print_tree_helper(t_tree *tree, char *prefix, int is_left)
{
	char	*new_prefix;
	t_token	*tok;

	if (!tree)
		return ;
	ft_printf("%s", prefix);
	if (is_left)
		ft_printf("├── ");
	else
		ft_printf("└── ");
	ft_printf("[%s]", toktype_str(tree->type));
	tok = tree->tokens;
	while (tok)
	{
		if (tok->value)
			ft_printf(" \"%s\"", tok->value);
		tok = tok->next;
	}
	ft_printf("\n");
	new_prefix = ft_strjoin(prefix, is_left ? "│   " : "    ");
	if (!new_prefix)
		return ;
	print_tree_helper(tree->left, new_prefix, 1);
	print_tree_helper(tree->right, new_prefix, 0);
	free(new_prefix);
}

void	print_tree(t_tree *tree)
{
	t_token	*tok;

	if (!tree)
	{
		ft_printf("(empty tree)\n");
		return ;
	}
	ft_printf("[%s]", toktype_str(tree->type));
	tok = tree->tokens;
	while (tok)
	{
		if (tok->value)
			ft_printf(" \"%s\"", tok->value);
		tok = tok->next;
	}
	ft_printf("\n");
	print_tree_helper(tree->left, "", 1);
	print_tree_helper(tree->right, "", 0);
}

void	free_tree(t_tree *tree)
{
	t_token	*tmp;

	if (!tree)
		return ;
	free_tree(tree->left);
	free_tree(tree->right);
	while (tree->tokens)
	{
		tmp = tree->tokens->next;
		free(tree->tokens->value);
		free(tree->tokens);
		tree->tokens = tmp;
	}
	free(tree);
}