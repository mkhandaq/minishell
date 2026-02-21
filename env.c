/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 15:38:37 by marvin            #+#    #+#             */
/*   Updated: 2026/02/20 15:38:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    env(char **str)
{
	int	i;

	i = 0;
	while(str[i])
	{
		ft_printf("%s\n", str[i]);
		i++;
	}
}

void	pwd(char **str)
{
	int	i;

	i = 0;
	while(str[i])
	{
		if(!ft_strncmp(str[i], "PWD=", 4))
		{
			ft_printf("%s\n", str[i] + 4);
			return ;
		}
		i++;
	}
}

char 	**export(char **env, char *added_var)
{
	int		i;
	char	**new_env;

	i = 0;
	while(env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if(!new_env)
		return (env);
	i = 0;
	while(env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = ft_strdup(added_var);
	new_env[i + 1] = NULL;
	i = 0;
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
	return (new_env);
}
