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

char	**export(char **env, char *added_var)
{
	int		i;
	int		name_len;
	char	**new_env;

	name_len = 0;
	while (added_var[name_len] && added_var[name_len] != '=')
		name_len++;
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], added_var, name_len)
			&& env[i][name_len] == '=')
		{
			free(env[i]);
			env[i] = ft_strdup(added_var);
			return (env);
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (env);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = ft_strdup(added_var);
	new_env[i + 1] = NULL;
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
	return (new_env);
}

static int	is_found(char **env, char *var)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], var, ft_strlen(var))
			&& env[i][ft_strlen(var)] == '=')
			return (1);
		i++;
	}
	return (0);
}

char	**unset(char **env, char *removed_var)
{
	int		i;
	int		j;
	int		size;
	char	**new_env;

	if (!is_found(env, removed_var))
		return (env);
	size = 0;
	while (env[size])
		size++;
	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		return (env);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], removed_var, ft_strlen(removed_var))
			|| env[i][ft_strlen(removed_var)] != '=')
		{
			new_env[j] = ft_strdup(env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
	return (new_env);
}
