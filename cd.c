/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:15:20 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:15:20 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_print_path_err(char *path, char *oldpwd)
{
	ft_putstr_fd("shellGuys: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	free(oldpwd);
	return (1);
}

static void	cd_update_pwd(char ***env, char *oldpwd, char *newpwd)
{
	char	*tmp;

	tmp = ft_strjoin("OLDPWD=", oldpwd);
	*env = export(*env, tmp);
	free(tmp);
	tmp = ft_strjoin("PWD=", newpwd);
	*env = export(*env, tmp);
	free(tmp);
	free(oldpwd);
	free(newpwd);
}

int	cd(char **args, char ***env)
{
	char	*oldpwd;
	char	*newpwd;
	char	*path;

	if (args[2])
	{
		ft_putstr_fd("shellGuys: cd: too many arguments\n", 2);
		return (1);
	}
	oldpwd = getcwd(NULL, 0);
	if (!args[1])
		path = ft_getenv(*env, "HOME");
	else
		path = args[1];
	if (!path)
	{
		ft_putstr_fd("shellGuys: cd: HOME not set\n", 2);
		free(oldpwd);
		return (1);
	}
	if (chdir(path) == -1)
		return (cd_print_path_err(path, oldpwd));
	newpwd = getcwd(NULL, 0);
	cd_update_pwd(env, oldpwd, newpwd);
	return (0);
}
