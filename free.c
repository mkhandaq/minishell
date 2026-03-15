/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:16:45 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:16:45 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_2d(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_list(t_token	**list)
{
	t_token	*tmp;

	tmp = *list;
	while (*list)
	{
		tmp = (*list)->next;
		free((*list)->value);
		free(*list);
		*list = tmp;
	}
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
