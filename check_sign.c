/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sign_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 23:50:02 by marvin            #+#    #+#             */
/*   Updated: 2026/02/06 23:50:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_two_sided(t_token *node)
{
	if(node && (node->type == TOK_AND || node->type == TOK_OR 
		|| node->type == TOK_PIPE))
		return (1);
	return (0);
}

int	is_sign(t_token *node)
{
	if(node && (node->type == TOK_AND || node->type == TOK_OR 
		|| node->type == TOK_PIPE || node->type == TOK_REDIR_IN
		|| node->type == TOK_REDIR_OUT || node->type == TOK_REDIR_APPEND
		|| node->type == TOK_HEREDOC))
		return (1);
	return (0);
}

int	is_one_sided(t_token *node)
{
	if(node && (node->type == TOK_REDIR_IN
		|| node->type == TOK_REDIR_OUT || node->type == TOK_REDIR_APPEND
		|| node->type == TOK_HEREDOC))
		return (1);
	return (0);
}

int	return_sign_len(t_token *node)
{
	if(node && (node->type == TOK_PIPE || node->type == TOK_REDIR_IN
		|| node->type == TOK_REDIR_OUT))
		return (1);
	else if(node && (node->type == TOK_AND || node->type == TOK_OR 
			|| node->type == TOK_REDIR_APPEND || node->type == TOK_HEREDOC))
			return (2);
	return (0);
}

void set_sign(t_token **node)
{
	if(node && (!ft_strncmp((*node)->value, "||", 2)))
		(*node)->type = TOK_OR;
	else if(node && (!ft_strncmp((*node)->value, "&&", 2)))
		(*node)->type = TOK_AND;
	else if(node && (!ft_strncmp((*node)->value, "|", 1)))
		(*node)->type = TOK_PIPE;
	else if(node && (!ft_strncmp((*node)->value, ">>", 2)))
		(*node)->type = TOK_REDIR_APPEND;
	else if(node && (!ft_strncmp((*node)->value, ">", 1)))
		(*node)->type = TOK_REDIR_OUT;
	else if(node && (!ft_strncmp((*node)->value, "<<", 2)))
		(*node)->type = TOK_HEREDOC;
	else if(node && (!ft_strncmp((*node)->value, "<", 1)))
		(*node)->type = TOK_REDIR_IN;
}

