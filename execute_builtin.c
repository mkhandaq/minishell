/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:15:57 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:15:58 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_export(t_token *cmd, char ***env)
{
	t_token	*tmp;
	int		found;
	int		ret;

	tmp = cmd->next;
	found = 0;
	ret = 0;
	while (tmp && !is_two_sided(tmp))
	{
		if (is_sign(tmp) || is_file(tmp) || !tmp->value)
		{
			tmp = tmp->next;
			continue ;
		}
		found = 1;
		ret += exec_export_var(tmp, env);
		tmp = tmp->next;
	}
	if (!found)
		print_export(*env);
	return (ret);
}

static int	exec_unset(t_token *cmd, char ***env)
{
	t_token	*tmp;
	int		ret;

	ret = 0;
	tmp = cmd->next;
	while (tmp && !is_two_sided(tmp))
	{
		if (is_sign(tmp) || is_file(tmp) || !tmp->value)
		{
			tmp = tmp->next;
			continue ;
		}
		if (!is_valid_identifier(tmp->value))
		{
			ft_putstr_fd("shellGuys: unset: `", 2);
			ft_putstr_fd(tmp->value, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else
			*env = unset(*env, tmp->value);
		tmp = tmp->next;
	}
	return (ret);
}

static int	exec_cd(t_token *cmd, char ***env)
{
	char	**args;
	int		ret;

	args = set_whole_command(cmd, cmd->value);
	if (!args)
		return (1);
	ret = cd(args, env);
	free_2d(args);
	return (ret);
}

int	execute_builtin(t_token *list, char ***envp, int *last_exit)
{
	t_token	*cmd;

	cmd = list;
	while (cmd && !is_built_in(cmd))
		cmd = cmd->next;
	if (!cmd)
		return (-1);
	if (cmd->type == TOK_ECHO)
		return (echo(&cmd));
	if (cmd->type == TOK_PWD)
		return (pwd());
	if (cmd->type == TOK_ENV)
		return (env(*envp));
	if (cmd->type == TOK_CD)
		return (exec_cd(cmd, envp));
	if (cmd->type == TOK_EXPORT)
		return (exec_export(cmd, envp));
	if (cmd->type == TOK_UNSET)
		return (exec_unset(cmd, envp));
	if (cmd->type == TOK_EXIT)
	{
		*last_exit = builtin_exit(cmd);
		return (*last_exit);
	}
	return (-1);
}
