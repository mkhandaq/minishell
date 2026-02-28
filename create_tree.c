/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 20:54:19 by aalemami          #+#    #+#             */
/*   Updated: 2026/02/27 20:54:19 by aalemami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// check header

#include "minishell.h"

int	has_command(t_token	*token_list) //check if the param is not null and has atleast one command;
{
	if (!token_list)
		return (1);
	while (token_list)
	{
		if (token_list->type == TOK_CMD)
			return (0);
		token_list = token_list->next;
	}
	return (1);
}


int	get_index(t_token *token_list)
{
	
}

int	search_for(t_token	*node, t_toktype type)
{
	int	i;

	i = 0;
	while (node)
	{
		if (node->type == type)
		{
			return (i);
		}
		node = node->next;
		i++;
	}
	return (-1);
}

t_tree	*split_tree_by_index(t_token *command_list, int *index)
{
	t_tree	node;
	int		i;
	node = command_list;
	node.start = index;
	
}
void	split_into_tree(t_token *command_line)
{
	t_tree		*command_tree;
	static int	start;
	static int	end;

	if (!command_line)
		return (1);
	if (search_for(command_line, TOK_OR) != -1)
	{
		end = search_for(command_line, TOK_OR);
		command_tree->left->command_list = command_line;
		command_tree->right->command_list = command_line;
		command_tree->start = end;
		command_tree->end = end;
	}
	else if (search_for(command_line, TOK_AND) != -1)
	{

	}
	else if (search_for(token_list, TOK_PIPE) != -1)
	{

	}
	else if (search_for(token_list, TOK_REDIR_APPEND) != -1)
	{

	}
	else if (search_for(token_list, TOK_REDIR_IN) != -1)
	{

	}
	else if (search_for(token_list, TOK_REDIR_OUT) != -1)
	{

	}
	else
	{
		commnad_tree->token_list = token_start;
	}
	index = 0;
}
