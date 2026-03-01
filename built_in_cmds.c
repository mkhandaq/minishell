/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 03:20:04 by marvin            #+#    #+#             */
/*   Updated: 2026/02/23 03:20:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_built_in_cmds(t_token **node)
{
	t_token *tmp;

	tmp = *node;
	while(tmp)
	{
		if (tmp && !ft_strncmp(tmp->value, "echo", 4) 
			&& ft_strlen(tmp->value) == 4)
			tmp->type = TOK_ECHO;
		else if (tmp && !ft_strncmp(tmp->value, "cd", 2) 
			&& ft_strlen(tmp->value) == 2)
			tmp->type = TOK_CD;
		else if (tmp && !ft_strncmp(tmp->value, "export", 6)
			&& ft_strlen(tmp->value) == 6)
			tmp->type = TOK_EXPORT;
		else if (tmp && !ft_strncmp(tmp->value, "unset", 5) 
			&& ft_strlen(tmp->value) == 5)
			tmp->type = TOK_UNSET;
		else if (tmp && !ft_strncmp(tmp->value, "env", 3) 
			&& ft_strlen(tmp->value) == 3)
			tmp->type = TOK_ENV;
		else if (tmp && !ft_strncmp(tmp->value, "pwd", 3) 
			&& ft_strlen(tmp->value) == 3)
			tmp->type = TOK_PWD;
		else if (tmp && !ft_strncmp(tmp->value, "exit", 4)
			&& ft_strlen(tmp->value) == 4)
			tmp->type = TOK_EXIT;
		if(is_sign(tmp))
			tmp = tmp->next;
		while(tmp && !is_sign(tmp))
			tmp = tmp->next;
	}
}

void	echo(t_token **list)
{
	int i;

	i = 0;
	if ((*list)->next && !ft_strncmp((*list)->next->value, "-n", 2) 
		&& ft_strlen((*list)->next->value) == 2)
	{
		*list = (*list)->next;
		i = 1;
	}
	*list = (*list)->next;
	while(*list && !(is_sign(*list)))
	{
		ft_printf("%s ", (*list)->value);
		*list = (*list)->next;
	}
	if(!i)
		ft_printf("\n");
}

int	cd(char **args, char ***env)
{
	char	*oldpwd;
	char	*newpwd;
	char	*path;
	char	*tmp;

	if (args[2])
	{
		ft_printf("shellGuys: cd: too many arguments\n");
		return (1);
	}
	oldpwd = getcwd(NULL, 0);
	if (!args[1])
		path = getenv("HOME");
	else
		path = args[1];
	if (!path)
	{
		ft_printf("shellGuys: cd: HOME not set\n");
		free(oldpwd);
		return (1);
	}
	if (chdir(path) == -1)
	{
		ft_printf("shellGuys: cd: %s: No such file or directory\n", path);
		free(oldpwd);
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	tmp = ft_strjoin("OLDPWD=", oldpwd);
	*env = export(*env, tmp);
	free(tmp);
	tmp = ft_strjoin("PWD=", newpwd);
	*env = export(*env, tmp);
	free(tmp);
	free(oldpwd);
	free(newpwd);
	return (0);
}