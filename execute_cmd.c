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

static int	process_redirections_only(t_token *list)
{
	int	stdin_copy;
	int	stdout_copy;
	int	status;

	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	if (stdin_copy == -1 || stdout_copy == -1)
	{
		if (stdin_copy != -1)
			close(stdin_copy);
		if (stdout_copy != -1)
			close(stdout_copy);
		return (1);
	}
	status = 0;
	if (!redirections(list))
		status = 1;
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
	return (status);
}

static int	process_built_in(t_exec_ctx *ctx,
		t_token *list, char *cmd, int *should_exit)
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
		*ctx->last_exit = 1;
		return (*ctx->last_exit);
	}
	if (!redirections(list))
	{
		dup_and_close(stdin_copy, stdout_copy, cmd);
		*ctx->last_exit = 1;
		return (*ctx->last_exit);
	}
	*ctx->last_exit = execute_builtin(list, ctx->env, ctx->last_exit,
			should_exit);
	dup_and_close(stdin_copy, stdout_copy, cmd);
	return (*ctx->last_exit);
}

static int	setup_execute_cmd(t_exec_ctx *ctx, t_token *list,
		char **cmd, int *should_exit)
{
	t_token	*tmp;

	expand_tokens(list, *ctx->last_exit, *ctx->env);
	*cmd = find_command(list);
	if (!*cmd)
	{
		*ctx->last_exit = process_redirections_only(list);
		return (0);
	}
	tmp = list;
	while (tmp)
	{
		if (is_built_in(tmp))
		{
			*ctx->last_exit = process_built_in(ctx, list, *cmd, should_exit);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	execute_cmd(char ***env, t_token *list, int *last_exit, int *should_exit)
{
	t_exec_ctx	ctx;
	char		*path;
	char		*cmd;
	char		**whole_cmd;

	ctx.env = env;
	ctx.last_exit = last_exit;
	if (!setup_execute_cmd(&ctx, list, &cmd, should_exit))
		return (*last_exit);
	path = get_path(cmd, *env);
	if (!path)
		*last_exit = exec_no_path(cmd);
	if (!path)
		return (*last_exit);
	whole_cmd = set_whole_command(list, cmd);
	free(cmd);
	*last_exit = 1;
	if (!whole_cmd)
		free(path);
	if (!whole_cmd)
		return (*last_exit);
	*last_exit = exec_fork(list, whole_cmd, env, path);
	free_2d(whole_cmd);
	free(path);
	return (*last_exit);
}
