/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:20:34 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/02/09 14:30:02 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	print_types(t_token	*node)
{
	while(node)
	{
		if(node->type == TOK_AND)
			ft_printf("[AND] ");
		else if(node->type == TOK_OR)
			ft_printf("[OR] ");
		else if(node->type == TOK_CMD)
			ft_printf("[CMD] ");
		else if(node->type == TOK_KEYWORD)
			ft_printf("[KEYWORD] ");
		else if(node->type == TOK_PIPE)
			ft_printf("[PIPE] ");
		else if(node->type == TOK_REDIR_APPEND)
			ft_printf("[REDIR_APPEND] ");
		else if(node->type == TOK_REDIR_IN)
			ft_printf("[REDIR_IN] ");
		else if(node->type == TOK_REDIR_OUT)
			ft_printf("[TOK_REDIR_OUT] ");
		else if(node->type == TOK_HEREDOC)
			ft_printf("[HEREDOC] ");
		if(node->next)
			ft_printf("-> ");
		node = node->next;
	}
	ft_printf("\n");
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

static void	free_2d(char **argv)
{
	int	i;

	i = 0;
	while(argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void printlist(t_token *node)
{
	t_token *tmp = node;
	while (tmp)
	{
		ft_printf("[%s] -> ", tmp -> value);
		tmp = tmp->next;
	}
	ft_printf("\n");
}

static void	shell_loop()
{
	char	*input;
	char	**splited;
	t_token	*node;

	node = NULL;
	while(1)
	{
		input = readline("shellGuys$ ");
		if (!input)
			return ;
		splited = ft_split(input, ' ');
		set_list(&node, splited);
		set_types(&node);
		print_types(node);
		printlist(node);
		free(input);
		free_2d(splited);
		free_list(&node);
	}
}

int main()
{
	shell_loop();
}