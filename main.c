/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:20:34 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/02/07 20:56:34 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv)
{
	t_token	*node = NULL;
	set_list(&node, argv);
	set_types(&node);
	while(node)
	{
		if(node->type == TOK_AND)
			ft_printf("[AND] -> ");
		else if(node->type == TOK_OR)
			ft_printf("[OR] -> ");
		else if(node->type == TOK_CMD)
			ft_printf("[CMD] -> ");
		else if(node->type == TOK_KEYWORD)
			ft_printf("[KEYWORD] -> ");
		else if(node->type == TOK_PIPE)
			ft_printf("[PIPE] -> ");
		else if(node->type == TOK_REDIR_APPEND)
			ft_printf("[REDIR_APPEND] -> ");
		else if(node->type == TOK_REDIR_IN)
			ft_printf("[REDIR_IN] -> ");
		else if(node->type == TOK_REDIR_OUT)
			ft_printf("[TOK_REDIR_OUT] -> ");
		else if(node->type == TOK_HEREDOC)
			ft_printf("[HEREDOC] -> ");
		node = node->next;
	}
}