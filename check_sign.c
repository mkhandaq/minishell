/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_sign.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:07:24 by ali_shell         #+#    #+#             */
/*   Updated: 2026/03/08 16:07:24 by ali_shell        ###   ########.fr       */
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
