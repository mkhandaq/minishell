/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:07:45 by ali_shell         #+#    #+#             */
/*   Updated: 2026/03/08 16:07:45 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_has_var(char **env, char *name)
{
	int	len;
	int	i;

	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, len)
			&& (env[i][len] == '=' || !env[i][len]))
			return (1);
		i++;
	}
	return (0);
}

char	*ft_getenv(char **env, const char *name)
{
	int	len;
	int	i;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, len) && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	**dup_env(char **env)
{
	int		size;
	int		i;
	char	**new_env;

	size = 0;
	while (env[size])
		size++;
	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[size] = NULL;
	return (new_env);
}

int	env(char **envt)
{
	int	i;

	i = 0;
	while (envt[i])
	{
		if (ft_printf("%s\n", envt[i]) == -1)
			return (1);
		i++;
	}
	return (0);
}

int	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("shellGuys: pwd: getcwd failed\n", 2);
		return (1);
	}
	if (ft_printf("%s\n", cwd) == -1)
	{
		free(cwd);
		return (1);
	}
	free(cwd);
	return (0);
}
