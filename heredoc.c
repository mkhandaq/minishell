/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:16:55 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/04/08 02:36:03 by aalemami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc_eof(t_token *tmp)
{
	if (g_signal == SIGINT)
		return (1);
	ft_putstr_fd("shellGuys: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(tmp->next->value, 2);
	ft_putstr_fd("')\n", 2);
	return (0);
}

static int	fill_heredoc(int fd, t_token *tmp,
		int exit_status, char **env)
{
	char	*line;
	char	*out;

	while (1)
	{
		line = read_heredoc_line();
		if (!line)
			return (handle_heredoc_eof(tmp));
		if (is_heredoc_limiter(line, tmp))
			return (0);
		out = line;
		if (tmp->next->strtype == TOK_STR)
			out = expand_heredoc_line(line, exit_status, env);
		write(fd, out, ft_strlen(out));
		write(fd, "\n", 1);
		free(out);
	}
	return (0);
}

static int	set_heredoc_target(char *filename, int stdin_backup)
{
	if (dup2(stdin_backup, STDIN_FILENO) == -1)
		return (close(stdin_backup), unlink(filename), free(filename), 1);
	close(stdin_backup);
	unlink(filename);
	free(filename);
	set_signals();
	return (1);
}

static int	handle_heredoc(t_token *tmp, int exit_status, char **env)
{
	char	*filename;
	int		fd;
	int		stdin_backup;
	int		interrupted;

	filename = gen_heredoc_name();
	if (!filename)
	{
		perror("heredoc filename");
		return (0);
	}
	fd = open_heredoc(filename);
	if (fd == -1)
		return (0);
	stdin_backup = dup(STDIN_FILENO);
	set_heredoc_signals();
	interrupted = fill_heredoc(fd, tmp, exit_status, env);
	close(fd);
	if (interrupted)
		return (set_heredoc_target(filename, stdin_backup));
	close(stdin_backup);
	set_signals();
	free(tmp->next->value);
	tmp->next->value = filename;
	return (0);
}

int	process_heredocs(t_token *list, int exit_status, char **env)
{
	while (list)
	{
		if (list->type == TOK_HEREDOC
			&& list->next && list->next->type == TOK_LIMITER)
		{
			if (handle_heredoc(list, exit_status, env))
				return (1);
		}
		list = list->next;
	}
	return (0);
}
