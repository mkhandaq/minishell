/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:36:47 by ialausud          #+#    #+#             */
/*   Updated: 2026/03/10 16:24:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*gen_heredoc_name(void)
{
	static int	idx = 0;
	char		*num;
	char		*name;
	char		*prefix;

	prefix = "/tmp/shellGuys_heredoc_";
	num = ft_itoa(idx++);
	if (!num)
		return (NULL);
	name = ft_strjoin(prefix, num);
	free(num);
	return (name);
}

static int	open_heredoc(char *filename)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("heredoc open");
		free(filename);
	}
	return (fd);
}

static void	fill_heredoc(int fd, char *limiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

static void	handle_heredoc(t_token *tmp)
{
	char	*filename;
	int		fd;

	filename = gen_heredoc_name();
	if (!filename)
		return (perror("heredoc filename"));
	fd = open_heredoc(filename);
	if (fd == -1)
		return ;
	fill_heredoc(fd, tmp->next->value);
	close(fd);
	free(tmp->next->value);
	tmp->next->value = filename;
}

void	process_heredocs(t_token *list)
{
	while (list)
	{
		if (list->type == TOK_HEREDOC
			&& list->next && list->next->type == TOK_LIMITER)
			handle_heredoc(list);
		list = list->next;
	}
}
