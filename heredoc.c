/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:36:47 by ialausud          #+#    #+#             */
/*   Updated: 2026/03/14 17:37:45 by marvin           ###   ########.fr       */
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

static char	*expand_heredoc_line(char *line, int exit_status, char **env)
{
	t_token	tmp;

	tmp.type = TOK_KEYWORD;
	tmp.strtype = TOK_STR;
	tmp.is_exuted = 0;
	tmp.value = line;
	tmp.next = NULL;
	expand_tokens(&tmp, exit_status, env);
	return (tmp.value);
}

static void	fill_heredoc(int fd, char *limiter, int do_expand,
		int exit_status, char **env)
{
	char	*line;
	char	*out;
	int		len;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
		{
			line = get_next_line(STDIN_FILENO);
			if (line)
			{
				len = ft_strlen(line);
				if (len > 0 && line[len - 1] == '\n')
					line[len - 1] = '\0';
			}
		}
		if (!line || !ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
		{
			free(line);
			break ;
		}
		if (do_expand)
			out = expand_heredoc_line(line, exit_status, env);
		else
			out = line;
		write(fd, out, ft_strlen(out));
		write(fd, "\n", 1);
		free(out);
	}
}

static void	handle_heredoc(t_token *tmp, int exit_status, char **env)
{
	char	*filename;
	int		fd;
	int		do_expand;

	filename = gen_heredoc_name();
	if (!filename)
		return (perror("heredoc filename"));
	fd = open_heredoc(filename);
	if (fd == -1)
		return ;
	do_expand = (tmp->next->strtype == TOK_STR);
	fill_heredoc(fd, tmp->next->value, do_expand, exit_status, env);
	close(fd);
	free(tmp->next->value);
	tmp->next->value = filename;
}

void	process_heredocs(t_token *list, int exit_status, char **env)
{
	while (list)
	{
		if (list->type == TOK_HEREDOC
			&& list->next && list->next->type == TOK_LIMITER)
			handle_heredoc(list, exit_status, env);
		list = list->next;
	}
}
