/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:16:16 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:16:16 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	dup_and_close(int stdin_copy, int stdout_copy, char *cmd)
{
	free(cmd);
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}

static int	process_built_in(char ***env,
	t_token *list, int *last_exit, char *cmd)
{
	int		stdin_copy;
	int		stdout_copy;

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
		dup_and_close(stdin_copy, stdout_copy, cmd);
		*last_exit = 1;
		return (*last_exit);
	}
	*last_exit = execute_builtin(list, env, last_exit);
	dup_and_close(stdin_copy, stdout_copy, cmd);
	return (*last_exit);
}

static int	setup_execute_cmd(char ***env, t_token *list,
		int *last_exit, char **cmd)
{
	expand_tokens(list, *last_exit, *env);
	*cmd = find_command(list);
	if (!*cmd)
	{
		*last_exit = 0;
		return (0);
	}
	if (is_built_in(list))
	{
		*last_exit = process_built_in(env, list, last_exit, *cmd);
		return (0);
	}
	return (1);
}

int	execute_cmd(char ***env, t_token *list, int *last_exit)
{
	char	*path;
	char	*cmd;
	char	**whole_cmd;

	if (!setup_execute_cmd(env, list, last_exit, &cmd))
		return (*last_exit);
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

// int	execute_cmd(char ***env, t_token *list, int *last_exit)
// {
// 	char	*path;
// 	char	*cmd;
// 	char	**whole_cmd;
// 	int		stdin_copy;
// 	int		stdout_copy;
// 	expand_tokens(list, *last_exit, *env);
// 	cmd = find_command(list);
// 	if (!cmd)
// 	{
// 		*last_exit = 0;
// 		return (*last_exit);
// 	}
// 	if (is_built_in(list))
// 	{
// 		stdin_copy = dup(STDIN_FILENO);
// 		stdout_copy = dup(STDOUT_FILENO);
// 		if (stdin_copy == -1 || stdout_copy == -1)
// 		{
// 			if (stdin_copy != -1)
// 				close(stdin_copy);
// 			if (stdout_copy != -1)
// 				close(stdout_copy);
// 			free(cmd);
// 			*last_exit = 1;
// 			return (*last_exit);
// 		}
// 		if (!redirections(list))
// 		{
// 			dup2(stdin_copy, STDIN_FILENO);
// 			dup2(stdout_copy, STDOUT_FILENO);
// 			close(stdin_copy);
// 			close(stdout_copy);
// 			free(cmd);
// 			*last_exit = 1;
// 			return (*last_exit);
// 		}
// 		free(cmd);
// 		*last_exit = execute_builtin(list, env, last_exit);
// 		dup2(stdin_copy, STDIN_FILENO);
// 		dup2(stdout_copy, STDOUT_FILENO);
// 		close(stdin_copy);
// 		close(stdout_copy);
// 		return (*last_exit);
// 	}
// 	path = get_path(cmd, *env);
// 	if (!path)
// 	{
// 		*last_exit = exec_no_path(cmd);
// 		return (*last_exit);
// 	}
// 	whole_cmd = set_whole_command(list, cmd);
// 	free(cmd);
// 	if (!whole_cmd)
// 	{
// 		free(path);
// 		*last_exit = 1;
// 		return (*last_exit);
// 	}
// 	*last_exit = exec_fork(list, whole_cmd, env, path);
// 	free_2d(whole_cmd);
// 	free(path);
// 	return (*last_exit);
// }
