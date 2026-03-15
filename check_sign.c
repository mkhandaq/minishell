/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_sign.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:15:29 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:15:29 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_two_sided(t_token *node)
{
	if (node && (node->type == TOK_AND
			|| node->type == TOK_OR
			|| node->type == TOK_PIPE))
		return (1);
	return (0);
}

int	is_sign(t_token *node)
{
	if (node && (node->type == TOK_AND || node->type == TOK_OR
			|| node->type == TOK_PIPE || node->type == TOK_REDIR_IN
			|| node->type == TOK_REDIR_OUT || node->type == TOK_REDIR_APPEND
			|| node->type == TOK_HEREDOC || node->type == TOK_OPENBRC
			|| node->type == TOK_CLOSEBRC))
		return (1);
	return (0);
}

int	is_one_sided(t_token *node)
{
	if (node && (node->type == TOK_REDIR_IN
			|| node->type == TOK_REDIR_OUT || node->type == TOK_REDIR_APPEND
			|| node->type == TOK_HEREDOC))
		return (1);
	return (0);
}

int	is_file(t_token *node)
{
	if (node && (node->type == TOK_INFILE || node->type == TOK_OUTFILE
			|| node->type == TOK_LIMITER))
		return (1);
	return (0);
}

int	return_sign_len(t_token *node)
{
	if (node && (node->type == TOK_PIPE || node->type == TOK_REDIR_IN
			|| node->type == TOK_REDIR_OUT || node->type == TOK_OPENBRC
			|| node->type == TOK_CLOSEBRC))
		return (1);
	else if (node && (node->type == TOK_AND || node->type == TOK_OR
			|| node->type == TOK_REDIR_APPEND || node->type == TOK_HEREDOC))
		return (2);
	return (0);
}
