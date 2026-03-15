/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:16:24 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:16:24 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_and(t_tree *tree, char ***env, int *last_exit)
{
	*last_exit = execute(tree->left, env, last_exit);
	if (*last_exit == 0)
		*last_exit = execute(tree->right, env, last_exit);
	return (*last_exit);
}

static int	execute_or(t_tree *tree, char ***env, int *last_exit)
{
	*last_exit = execute(tree->left, env, last_exit);
	if (*last_exit != 0)
		*last_exit = execute(tree->right, env, last_exit);
	return (*last_exit);
}

static int	execute_subshell(t_tree *tree, char ***env, int *last_exit)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (1);
	}
	if (pid == 0)
	{
		*last_exit = execute(tree->left, env, last_exit);
		exit(*last_exit);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*last_exit = WEXITSTATUS(status);
	return (*last_exit);
}

int	execute(t_tree *tree, char ***env, int *last_exit)
{
	if (!tree)
		return (0);
	if (tree->type == TOK_PIPE)
		return (execute_pipe(tree, env, last_exit));
	if (tree->type == TOK_AND)
		return (execute_and(tree, env, last_exit));
	if (tree->type == TOK_OR)
		return (execute_or(tree, env, last_exit));
	if (tree->type == TOK_CMD)
		return (execute_cmd(env, tree->tokens, last_exit));
	if (tree->type == TOK_SUBSHELL)
		return (execute_subshell(tree, env, last_exit));
	return (0);
}
