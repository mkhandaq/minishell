/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:17:42 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:17:42 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*shell_read(int last_exit)
{
	char	*input;
	int		len;

	if (isatty(STDIN_FILENO))
		input = readline(GREEN "shellGuys" BLUE "$ " RESET);
	else
	{
		input = get_next_line(STDIN_FILENO);
		if (input)
		{
			len = ft_strlen(input);
			if (len > 0 && input[len - 1] == '\n')
				input[len - 1] = '\0';
		}
	}
	if (!input)
	{
		ft_printf("exit\n");
		exit(last_exit);
	}
	if (isatty(STDIN_FILENO) && input[0] != '\0')
		add_history(input);
	return (input);
}

static void	shell_exec(t_token *node, t_tree **tree,
		char ***env, int *last_exit)
{
	process_heredocs(node, *last_exit, *env);
	expand_wildcards(node);
	set_built_in_cmds(&node);
	*tree = build_tree(node);
	execute(*tree, env, last_exit);
	free_tree(*tree);
}

void	shell_loop(char **env)
{
	char	*input;
	t_token	*node;
	t_tree	*tree;
	int		last_exit;

	last_exit = 0;
	while (1)
	{
		input = shell_read(last_exit);
		node = set_list(input);
		free(input);
		if (!set_types(&node))
		{
			last_exit = 2;
			free_list(&node);
			continue ;
		}
		shell_exec(node, &tree, &env, &last_exit);
	}
}
