/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:07:29 by ali_shell         #+#    #+#             */
/*   Updated: 2026/03/08 16:07:29 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_2d (char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

char	**set_whole_command(t_token *list, char *cmd)
{
	t_token	*tmp;
	char	**whole_command;
	int		i;

	i = 1;
	tmp = list;
	while (tmp)
	{
		if (tmp->type == TOK_KEYWORD)
			i++;
		tmp = tmp->next;
	}
	whole_command = malloc(sizeof(char *) * (i + 1));
	if (!whole_command)
		return (NULL);
	whole_command[0] = ft_strdup(cmd);
	i = 1;
	tmp = list;
	while (tmp)
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

int	redirections(t_token *list)
{
	t_token	*tmp;
	int		fd;

	tmp = list;
	while (tmp)
	{
		if (tmp->type == TOK_REDIR_IN && tmp->next)
		{
			fd = open(tmp->next->value, O_RDONLY);
			if (fd == -1)
			{
				ft_printf("shellGuys: %s: No such file or directory\n",
					tmp->next->value);
				return (0);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (tmp->type == TOK_REDIR_OUT && tmp->next)
		{
			fd = open(tmp->next->value,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				ft_printf("shellGuys: %s: No such file or directory\n",
					tmp->next->value);
				return (0);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (tmp->type == TOK_REDIR_APPEND && tmp->next)
		{
			fd = open(tmp->next->value,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				ft_printf("shellGuys: %s: No such file or directory\n",
					tmp->next->value);
				return (0);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (tmp->type == TOK_HEREDOC && tmp->next)
		{
			fd = open(tmp->next->value, O_RDONLY);
			if (fd == -1)
				return (0);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		tmp = tmp->next;
	}
	return (1);
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