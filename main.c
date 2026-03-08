/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:20:34 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/08 06:58:45 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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

static void	shell_loop(char **env)
{
	char	*input;
	t_token	*node;
	t_tree	*tree;
	int		last_exit;

	last_exit = 0;
	while(1)
	{
		input = readline(GREEN "shellGuys" BLUE "$ " RESET);
		if (!input)
		{
			ft_printf("exit\n");
			exit(last_exit);
		}
		if (input[0] != '\0')
			add_history(input);
		node = set_list(input);
		set_types(&node);
		process_heredocs(node);
		expand_tokens(node, last_exit);
		set_built_in_cmds(&node);
		tree = build_tree(node);
		execute(tree, &env, &last_exit);
		free_tree(tree);
	}
}

int main(int ac, char **av, char **env)
{
	if(ac != 1 || !(av || *av))
		return (1);
	set_signals();
	shell_loop(env);
}

//// to do list

// add subshell token
// implement exit
// parse built in commands number of parameters? test 
// expansion
// 
// execute 
// 
// verify headers and make sure they are correct