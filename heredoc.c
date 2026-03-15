/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:36:47 by ialausud          #+#    #+#             */
/*   Updated: 2026/03/15 09:10:15 by aalemami         ###   ########.fr       */
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

static void	fill_heredoc(int fd, t_token *tmp,
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
		if (!line || !ft_strncmp(line, tmp->next->value,
				ft_strlen(tmp->next->value) + 1))
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

	filename = gen_heredoc_name();
	if (!filename)
		return (perror("heredoc filename"));
	fd = open_heredoc(filename);
	if (fd == -1)
		return ;
	fill_heredoc(fd, tmp, exit_status, env);
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

// static void	fill_heredoc(int fd, char *limiter, int do_expand,
// 		int exit_status, char **env)
// {
// 	char	*line;
// 	char	*out;
// 	int		len;
// 	while (1)
// 	{
// 		if (isatty(STDIN_FILENO))
// 			line = readline("> ");
// 		else
// 		{
// 			line = get_next_line(STDIN_FILENO);
// 			if (line)
// 			{
// 				len = ft_strlen(line);
// 				if (len > 0 && line[len - 1] == '\n')
// 					line[len - 1] = '\0';
// 			}
// 		}
// 		if (!line || !ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (do_expand)
// 			out = expand_heredoc_line(line, exit_status, env);
// 		else
// 			out = line;
// 		write(fd, out, ft_strlen(out));
// 		write(fd, "\n", 1);
// 		free(out);
// 	}
// }
