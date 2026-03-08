/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:08:25 by ali_shell         #+#    #+#             */
/*   Updated: 2026/03/08 16:08:25 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	left_child(int fd[2], t_tree *tree, char ***env, int *last_exit)
{
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	exit(execute(tree->left, env, last_exit));
}

static void	right_child(int fd[2], t_tree *tree, char ***env, int *last_exit)
{
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	exit(execute(tree->right, env, last_exit));
}

int	execute_pipe(t_tree *tree, char ***env, int *last_exit)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(fd) == -1)
	{
		perror("pipe failed");
		return (1);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork failed");
		close(fd[0]);
		close(fd[1]);
		return (1);
	}
	if (pid1 == 0)
		left_child(fd, tree, env, last_exit);
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork failed");
		close(fd[0]);
		close(fd[1]);
		return (1);
	}
	if (pid2 == 0)
		right_child(fd, tree, env, last_exit);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		*last_exit = WEXITSTATUS(status);
	return (*last_exit);
}