/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:36:47 by ialausud          #+#    #+#             */
/*   Updated: 2026/03/09 18:46:13 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

static char *gen_heredoc_name(void)
{
	static int	idx = 0;
	char		*num;
	char		*name;
	char		*prefix = "/tmp/shellGuys_heredoc_";

	num = ft_itoa(idx++);
	if (!num)
	return (NULL);
	name = ft_strjoin(prefix, num);
	free(num);
	return (name);
}

void    process_heredocs(t_token *list)
{
	t_token	*tmp = list;
	char	*limiter;
	char	*filename;
	int		fd;
	char	*line;

	while (tmp)
	{
		if (tmp->type == TOK_HEREDOC
			&& tmp->next && tmp->next->type == TOK_LIMITER)
		{
			limiter = tmp->next->value;
			filename = gen_heredoc_name();
			if (!filename)
			{
				perror("heredoc filename");
				return;
			}
			fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("heredoc open");
				free(filename);
				return;
			}
			while (1)
			{
				line = readline("> ");
				if (!line || !ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
				{
					free(line);
					break;
				}
				write(fd, line, ft_strlen(line));
				write(fd, "\n", 1);
				free(line);
			}
			close(fd);
			tmp->type = TOK_HEREDOC;
			tmp->next->type = TOK_LIMITER;
			free(tmp->next->value);
			tmp->next->value = filename;
		}
		tmp = tmp->next;
	}
}
