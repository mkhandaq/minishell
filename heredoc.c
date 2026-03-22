/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:16:55 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/23 02:38:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_heredoc_helper(char **line)
{
	int	len;

	*line = get_next_line(STDIN_FILENO);
	if (*line)
	{
		len = ft_strlen(*line);
		if (len > 0 && (*line)[len - 1] == '\n')
			(*line)[len - 1] = '\0';
	}
}

static int	fill_heredoc(int fd, t_token *tmp,
		int exit_status, char **env)
{
	char	*line;
	char	*out;
	int		do_expand;

	do_expand = (tmp->next->strtype == TOK_STR);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			fill_heredoc_helper(&line);
		if (!line)
		{
			ft_putstr_fd("shellGuys: warning: ", 2);
			ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(tmp->next->value, 2);
			ft_putstr_fd("')\n", 2);
			if (g_signal == SIGINT)
				return (1);
			break ;
		}
		if (!ft_strncmp(line, tmp->next->value, ft_strlen(tmp->next->value) + 1))
		{
			free(line);
			return (0);
		}
		if (do_expand)
			out = expand_heredoc_line(line, exit_status, env);
		else
			out = line;
		write(fd, out, ft_strlen(out));
		write(fd, "\n", 1);
		free(out);
	}
	return (0);
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
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
		unlink(filename);
		free(filename);
		set_signals();
		return (1);
	}
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
