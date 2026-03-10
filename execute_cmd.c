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

static void	exec_child(t_token *list, char **whole_cmd, char **env)
{
	if (!redirections(list))
		exit(1);
	execve(whole_cmd[0], whole_cmd, env);
	ft_putstr_fd("shellGuys: permission denied\n", 2);
	exit(126);
}

static int	exec_fork(t_token *list, char **whole_cmd, char ***env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		exec_child(list, whole_cmd, *env);
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

	cmd = find_command(list);
	if (!cmd)
		return (0);
	if (is_built_in(list))
	{
		free(cmd);
		return (execute_builtin(list, env, last_exit));
	}
	path = get_path(cmd, *env);
	if (!path)
		return (exec_no_path(cmd));
	whole_cmd = set_whole_command(list, cmd);
	free(cmd);
	if (!whole_cmd)
		return (free(path), 0);
	*last_exit = exec_fork(list, whole_cmd, env);
	free_2d(whole_cmd);
	free(path);
	return (*last_exit);
}
