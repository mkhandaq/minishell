/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 10:41:17 by marvin            #+#    #+#             */
/*   Updated: 2026/03/07 10:41:17 by marvin           ###   ########.fr       */
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
	char *cmd;

	while(list)
	{
		if (list->type == TOK_CMD)
			cmd = ft_strdup(list->value);
		list = list->next;
	}
	if (cmd)
		return (cmd);
	return (NULL);
}

int	execute_cmd(char **ev, t_token *list, int *last_exit)
{
	char	*path;
	char	*cmd;
	char	**whole_cmd;
	pid_t	pid;
	int		status;

	cmd = find_command(list);
	if (!cmd)
		return 0;
	if (is_built_in(list))
		return (1);
	path = get_path(cmd, ev);
	if (!path)
	{
		ft_printf("shellGuys: %s: command not found\n", cmd);
		return 0;
	}
	whole_cmd = set_whole_command(list, cmd);
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
    	execve(path, whole_cmd, ev);
		ft_printf("shellGuys: %s: permission denied\n", cmd);
	exit(126);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*last_exit = WEXITSTATUS(status);
	free_2d(whole_cmd);
	free(path);
	return (*last_exit);
}
