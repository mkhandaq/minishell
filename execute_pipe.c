/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:16:19 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:16:19 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	left_child(int fd[2], t_tree *tree, t_exec_ctx *ctx)
{
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	exit(execute(tree->left, ctx->env, ctx->last_exit, ctx->should_exit));
}

static void	right_child(int fd[2], t_tree *tree, t_exec_ctx *ctx)
{
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	exit(execute(tree->right, ctx->env, ctx->last_exit, ctx->should_exit));
}

static int	fork_failed(int *fd)
{
	perror("fork failed");
	close(fd[0]);
	close(fd[1]);
	return (1);
}

static int	wait_pipe(pid_t pid1, pid_t pid2, int *last_exit, int fd[2])
{
	int	status;

	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		*last_exit = WEXITSTATUS(status);
	return (*last_exit);
}

int	execute_pipe(t_tree *tree, char ***env, int *last_exit, int *should_exit)
{
	t_exec_ctx	ctx;
	int			fd[2];
	pid_t		pid1;
	pid_t		pid2;

	ctx.env = env;
	ctx.last_exit = last_exit;
	ctx.should_exit = should_exit;
	if (pipe(fd) == -1)
		return (perror("pipe failed"), 1);
	pid1 = fork();
	if (pid1 == -1)
		return (fork_failed(fd));
	if (pid1 == 0)
		left_child(fd, tree, &ctx);
	pid2 = fork();
	if (pid2 == -1)
		return (fork_failed(fd));
	if (pid2 == 0)
		right_child(fd, tree, &ctx);
	return (wait_pipe(pid1, pid2, last_exit, fd));
}
