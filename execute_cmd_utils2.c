/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:16:09 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/04/07 09:17:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command(t_token *list)
{
	while (list)
	{
		if ((list->type == TOK_CMD || is_built_in(list)) && list->value)
			return (ft_strdup(list->value));
		list = list->next;
	}
	return (NULL);
}

int	exec_no_path(char *cmd)
{
	char	*s1;
	char	*s2;

	s1 = ft_strdup("shellGuys: ");
	s2 = ft_strjoin(s1, cmd);
	free(s1);
	s1 = ft_strjoin (s2, ": command not found\n");
	write(2, s1, ft_strlen(s1));
	free(s1);
	free(s2);
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
		signal(SIGQUIT, SIG_DFL);
		exec_child(list, whole_cmd, *env, path);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	set_signals();
	if (WIFSIGNALED(status))
	{
		g_signal = WTERMSIG(status);
		if (g_signal == SIGINT)
			write(1, "\n", 1);
		else if (g_signal == SIGQUIT)
			write(2, "Quit: 3\n", 8);
		return (128 + g_signal);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}
