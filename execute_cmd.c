/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 14:58:02 by marvin            #+#    #+#             */
/*   Updated: 2026/03/10 14:58:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command(t_token *list)
{
	char	*cmd;

	cmd = NULL;
	while (list)
	{
		if (list->type == TOK_CMD || is_built_in(list))
		{
			free(cmd);
			cmd = ft_strdup(list->value);
		}
		list = list->next;
	}
	return (cmd);
}

static int	exec_no_path(char *cmd)
{
	ft_putstr_fd("shellGuys: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	free(cmd);
	return (127);
}

static void	exec_child(t_token *list, char **whole_cmd, char **env, char *path)
{
	if (!redirections(list))
		exit(1);
	execve(path, whole_cmd, env);
	ft_putstr_fd("shellGuys: permission denied\n", 2);
	exit(126);
}

static int	exec_fork(t_token *list, char **whole_cmd, char ***env, char *path)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		exec_child(list, whole_cmd, *env, path);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	execute_cmd(char ***env, t_token *list, int *last_exit)
{
	char	*path;
	char	*cmd;
	char	**whole_cmd;
	int		stdin_copy;
	int		stdout_copy;

	expand_tokens(list, *last_exit, *env);
	cmd = find_command(list);
	if (!cmd)
	{
		*last_exit = 0;
		return (*last_exit);
	}
	if (is_built_in(list))
	{
		stdin_copy = dup(STDIN_FILENO);
		stdout_copy = dup(STDOUT_FILENO);
		if (stdin_copy == -1 || stdout_copy == -1)
		{
			if (stdin_copy != -1)
				close(stdin_copy);
			if (stdout_copy != -1)
				close(stdout_copy);
			free(cmd);
			*last_exit = 1;
			return (*last_exit);
		}
		if (!redirections(list))
		{
			dup2(stdin_copy, STDIN_FILENO);
			dup2(stdout_copy, STDOUT_FILENO);
			close(stdin_copy);
			close(stdout_copy);
			free(cmd);
			*last_exit = 1;
			return (*last_exit);
		}
		free(cmd);
		*last_exit = execute_builtin(list, env, last_exit);
		dup2(stdin_copy, STDIN_FILENO);
		dup2(stdout_copy, STDOUT_FILENO);
		close(stdin_copy);
		close(stdout_copy);
		return (*last_exit);
	}
	path = get_path(cmd, *env);
	if (!path)
	{
		*last_exit = exec_no_path(cmd);
		return (*last_exit);
	}
	whole_cmd = set_whole_command(list, cmd);
	free(cmd);
	if (!whole_cmd)
	{
		free(path);
		*last_exit = 1;
		return (*last_exit);
	}
	*last_exit = exec_fork(list, whole_cmd, env, path);
	free_2d(whole_cmd);
	free(path);
	return (*last_exit);
}
