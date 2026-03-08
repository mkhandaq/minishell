/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:07:34 by ali_shell         #+#    #+#             */
/*   Updated: 2026/03/08 16:07:34 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	execute_builtin()
// {
// 	ft_printf("Under working\n");
// 	return (0);
// }

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

int	execute_cmd(char ***env, t_token *list, int *last_exit)
{
	char	*path;
	char	*cmd;
	char	**whole_cmd;
	pid_t	pid;
	int		status;

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
	{
		ft_putstr_fd("shellGuys: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		free(cmd);
		return (127);
	}
	whole_cmd = set_whole_command(list, cmd);
	free(cmd);
	if (!whole_cmd)
	{
		free(path);
		return (0);
	}
	pid = fork();
	if (pid == 0)
	{
		if (!redirections(list))
			exit(1);
		execve(path, whole_cmd, *env);
		ft_putstr_fd("shellGuys: permission denied\n", 2);
		exit(126);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*last_exit = WEXITSTATUS(status);
	free_2d(whole_cmd);
	free(path);
	return (*last_exit);
}
