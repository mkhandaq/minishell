/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:16:35 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:16:35 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	**export_new_env(char **env, char *added_var, int i)
{
	char	**new_env;
	int		j;

	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (env);
	j = 0;
	while (env[j])
	{
		new_env[j] = ft_strdup(env[j]);
		j++;
	}
	new_env[j] = ft_strdup(added_var);
	new_env[j + 1] = NULL;
	j = 0;
	while (env[j])
		free(env[j++]);
	free(env);
	return (new_env);
}

char	**export(char **env, char *added_var)
{
	int		i;
	int		name_len;

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
	return (export_new_env(env, added_var, i));
}

static char	**unset_build_env(char **env, char *removed_var, int size)
{
	char	**new_env;
	int		i;
	int		j;

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

char	**unset(char **env, char *removed_var)
{
	int		size;

	if (!is_found(env, removed_var))
		return (env);
	size = 0;
	while (env[size])
		size++;
	return (unset_build_env(env, removed_var, size));
}
