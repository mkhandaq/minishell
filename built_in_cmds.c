/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:07:14 by ali_shell         #+#    #+#             */
/*   Updated: 2026/03/08 16:07:14 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built_in(t_token *node)
{
	if (node && (node->type == TOK_ECHO || node->type == TOK_CD
			|| node->type == TOK_EXIT || node->type == TOK_EXPORT
			|| node->type == TOK_UNSET || node->type == TOK_PWD
			|| node->type == TOK_ENV))
		return (1);
	return (0);
}

int	set_sign(t_token **node)
{
	if (node && (!ft_strncmp((*node)->value, "||", 2)))
		(*node)->type = TOK_OR;
	else if (node && (!ft_strncmp((*node)->value, "&&", 2)))
		(*node)->type = TOK_AND;
	else if (node && (!ft_strncmp((*node)->value, "|", 1)))
		(*node)->type = TOK_PIPE;
	else if (node && (!ft_strncmp((*node)->value, ">>", 2)))
		(*node)->type = TOK_REDIR_APPEND;
	else if (node && (!ft_strncmp((*node)->value, ">", 1)))
		(*node)->type = TOK_REDIR_OUT;
	else if (node && (!ft_strncmp((*node)->value, "<<", 2)))
		(*node)->type = TOK_HEREDOC;
	else if (node && (!ft_strncmp((*node)->value, "<", 1)))
		(*node)->type = TOK_REDIR_IN;
	else if (node && (!ft_strncmp((*node)->value, "(", 1)))
		(*node)->type = TOK_OPENBRC;
	else if (node && (!ft_strncmp((*node)->value, ")", 1)))
		(*node)->type = TOK_CLOSEBRC;
	else
		return (0);
	return (1);
}

static int	get_builtin_type(const char *val)
{
	if (!ft_strncmp(val, "echo", 4) && ft_strlen(val) == 4)
		return (TOK_ECHO);
	if (!ft_strncmp(val, "cd", 2) && ft_strlen(val) == 2)
		return (TOK_CD);
	if (!ft_strncmp(val, "export", 6) && ft_strlen(val) == 6)
		return (TOK_EXPORT);
	if (!ft_strncmp(val, "unset", 5) && ft_strlen(val) == 5)
		return (TOK_UNSET);
	if (!ft_strncmp(val, "env", 3) && ft_strlen(val) == 3)
		return (TOK_ENV);
	if (!ft_strncmp(val, "pwd", 3) && ft_strlen(val) == 3)
		return (TOK_PWD);
	if (!ft_strncmp(val, "exit", 4) && ft_strlen(val) == 4)
		return (TOK_EXIT);
	return (-1);
}

void	set_built_in_cmds(t_token **node)
{
	t_token	*tmp;
	int		type;

	tmp = *node;
	while (tmp)
	{
		type = get_builtin_type(tmp->value);
		if (type != -1)
			tmp->type = type;
		if (is_sign(tmp))
		{
			tmp = tmp->next;
			continue ;
		}
		while (tmp && !is_sign(tmp))
			tmp = tmp->next;
	}
}
