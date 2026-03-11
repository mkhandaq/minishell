/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_types.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:08:36 by ali_shell         #+#    #+#             */
/*   Updated: 2026/03/11 00:00:00 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_file_types(t_token *tmp)
{
	if (tmp->type == TOK_REDIR_IN)
		tmp->next->type = TOK_INFILE;
	else if (tmp->type == TOK_HEREDOC)
		tmp->next->type = TOK_LIMITER;
	else
		tmp->next->type = TOK_OUTFILE;
}

static void	set_keyword_types(t_token *tmp)
{
	tmp = tmp->next;
	while (tmp && !is_two_sided(tmp))
	{
		if (!is_sign(tmp) && !is_file(tmp))
			tmp->type = TOK_KEYWORD;
		tmp = tmp->next;
	}
}

static void	set_cmd_types(t_token *tmp)
{
	while (tmp && tmp->next)
	{
		if (is_one_sided(tmp) && !is_sign(tmp->next))
			set_file_types(tmp);
		if (is_sign(tmp) && !is_one_sided(tmp) && !is_sign(tmp->next))
			tmp->next->type = TOK_CMD;
		if (is_file(tmp) && !is_sign(tmp->next))
			tmp->next->type = TOK_CMD;
		tmp = tmp->next;
	}
}

static void	set_keywords_pass(t_token *tmp)
{
	if (tmp && !is_sign(tmp))
		tmp->type = TOK_CMD;
	while (tmp)
	{
		if (tmp->type == TOK_CMD)
			set_keyword_types(tmp);
		if (tmp)
			tmp = tmp->next;
	}
}

int	set_types(t_token **list)
{
	t_token	*tmp;

	tmp = *list;
	while (tmp)
	{
		set_sign(&tmp);
		tmp = tmp->next;
	}
	tmp = *list;
	split_node_loop(&tmp);
	if (!check_syntax_errors(tmp))
		return (0);
	set_cmd_types(tmp);
	set_keywords_pass(tmp);
	tmp = *list;
	while (tmp && tmp->next)
	{
		if (tmp->type == TOK_CMD && !(is_sign(tmp->next)))
			tmp->next->type = TOK_KEYWORD;
		tmp = tmp->next;
	}
	tmp = *list;
	set_built_in_cmds(&tmp);
	return (1);
}
