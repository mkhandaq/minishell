/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:20:34 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/02/16 18:11:18 by mkhandaq         ###   ########.fr       */
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
			ft_printf("[REDIR_OUT] ");
		else if(node->type == TOK_HEREDOC)
			ft_printf("[HEREDOC] ");
		// here is some edits by me to print the types of the nodes after the signs
			// so here we print the types of the nodes after the signs
		else if(node->type == TOK_INFILE)
			ft_printf("[INFILE] ");
		else if(node->type == TOK_OUTFILE)
			ft_printf("[OUTFILE] ");
		else if(node->type == TOK_LIMITER)
			ft_printf("[LIMITER] ");
		else if(node->type == TOK_OPENBRC)
			ft_printf("[OPENBRC] ");
		else if(node->type == TOK_CLOSEBRC)
			ft_printf("[CLOSEBRC] ");
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
		if(is_sign(tmp))
		{
			if(tmp->type == TOK_AND)
				ft_printf("[&&] ");
			else if(tmp->type == TOK_OR)
				ft_printf("[||] ");
			else if(tmp->type == TOK_PIPE)
				ft_printf("[|] ");
			else if(tmp->type == TOK_REDIR_APPEND)
				ft_printf("[>>] ");
			else if(tmp->type == TOK_REDIR_IN)
				ft_printf("[<] ");
			else if(tmp->type == TOK_REDIR_OUT)
				ft_printf("[>] ");
			else if(tmp->type == TOK_HEREDOC)
				ft_printf("[<<] ");
			else if(tmp->type == TOK_OPENBRC)
				ft_printf("[(] ");
			else if(tmp->type == TOK_CLOSEBRC)
				ft_printf("[)] ");
				
		}
		else
			ft_printf("[%s] ", tmp->value);
		if(tmp->next)
			ft_printf("-> ");
		tmp = tmp->next;
	}
	ft_printf("\n");
}

static void	shell_loop()
{
	char	*input;
	char	**splited;
	t_token	*node;

	while(1)
	{
		input = readline("shellGuys$ ");
		if (!input)
			return ;
		splited = ft_split(input, ' ');
		node = NULL;
		set_list(&node, splited);
		set_types(&node);
		process_heredocs(node);
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