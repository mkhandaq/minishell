/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:16:03 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:16:03 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_keywords(t_token *list)
{
	int		i;

	i = 1;
	while (list && !is_two_sided(list))
	{
		if (list->type == TOK_KEYWORD)
			i++;
		list = list->next;
	}
	return (i);
}

char	**set_whole_command(t_token *list, char *cmd)
{
	char	**whole_command;
	t_token	*tmp;
	int		i;

	whole_command = malloc(sizeof(char *) * (count_keywords(list) + 1));
	if (!whole_command)
		return (NULL);
	whole_command[0] = ft_strdup(cmd);
	i = 1;
	tmp = list;
	while (tmp && !is_two_sided(tmp))
	{
		if (tmp->type == TOK_KEYWORD)
		{
			whole_command[i] = ft_strdup(tmp->value);
			i++;
		}
		tmp = tmp->next;
	}
	whole_command[i] = NULL;
	return (whole_command);
}

static char	*get_path_helper(char **path, char *cmd)
{
	int		i;
	char	*whole_path;
	char	*tmp;

	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		whole_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!access(whole_path, X_OK))
		{
			free_2d(path);
			return (whole_path);
		}
		free(whole_path);
		i++;
	}
	free_2d(path);
	return (NULL);
}

char	*get_path(char *cmd, char **ev)
{
	char	**path;
	int		i;

	if (!cmd || !ev)
		return (NULL);
	if (!access(cmd, X_OK))
		return (ft_strdup(cmd));
	i = 0;
	while (ev[i] && ft_strncmp(ev[i], "PATH=", 5))
		i++;
	if (!ev[i])
		return (NULL);
	path = ft_split(ev[i] + 5, ':');
	if (!path || !(*path))
		return (NULL);
	return (get_path_helper(path, cmd));
}
