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

static char	*cd_get_path(char **args, char **env)
{
	if (!args[1])
		return (ft_getenv(env, "HOME"));
	return (args[1]);
}

static int	cd_set_target(char **args, char **env,
		char **oldpwd, char **path)
{
	*oldpwd = getcwd(NULL, 0);
	if (!*oldpwd)
		*oldpwd = ft_strdup(ft_getenv(env, "PWD"));
	*path = cd_get_path(args, env);
	if (!*path)
	{
		ft_putstr_fd("shellGuys: cd: HOME not set\n", 2);
		free(*oldpwd);
		return (1);
	}
	return (0);
}

int	cd(char **args, char ***env)
{
	char	*oldpwd;
	char	*newpwd;
	char	*path;

	if (args[1] && args[2])
	{
		ft_putstr_fd("shellGuys: cd: too many arguments\n", 2);
		return (1);
	}
	if (cd_set_target(args, *env, &oldpwd, &path))
		return (1);
	if (chdir(path) == -1)
		return (cd_print_path_err(path, oldpwd));
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		newpwd = ft_strdup(path);
	cd_update_pwd(env, oldpwd, newpwd);
	return (0);
}
