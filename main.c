/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:20:34 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/08 15:42:24 by ali_shell        ###   ########.fr       */
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
		free(input);
		if (!set_types(&node))
		{
			last_exit = 2;
			free_list(&node);
			continue ;
		}
		process_heredocs(node);
		expand_tokens(node, last_exit, env);
		set_built_in_cmds(&node);
		tree = build_tree(node);
		execute(tree, &env, &last_exit);
		free_tree(tree);
	}
}

static char	**dup_env(char **env)
{
	int		size;
	int		i;
	char	**new_env;

	size = 0;
	while (env[size])
		size++;
	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[size] = NULL;
	return (new_env);
}

int	main(int ac, char **av, char **env)
{
	char	**env_copy;

	if (ac != 1 || !(av || *av))
		return (1);
	env_copy = dup_env(env);
	if (!env_copy)
		return (1);
	set_signals();
	shell_loop(env_copy);
}

// verify headers and make sure they are correct