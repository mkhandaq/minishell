/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:16:09 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/21 23:27:21 by aalemami         ###   ########.fr       */
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

int	exec_no_path(char *cmd)
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

int	exec_fork(t_token *list, char **whole_cmd, char ***env, char *path)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		exec_child(list, whole_cmd, *env, path);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	set_signals();
	if (WIFSIGNALED(status))
	{
		g_signal = WTERMSIG(status);
		write(1, "\n", 1);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}
