/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:19:24 by aalemami          #+#    #+#             */
/*   Updated: 2026/04/08 02:35:55 by aalemami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_heredoc_limiter(char *line, t_token *tmp)
{
	if (!ft_strncmp(line, tmp->next->value, ft_strlen(tmp->next->value) + 1))
	{
		free(line);
		return (1);
	}
	return (0);
}

char	*read_heredoc_line(void)
{
	char	*line;
	int		len;

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
	return (line);
}

char	*gen_heredoc_name(void)
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

int	open_heredoc(char *filename)
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

char	*expand_heredoc_line(char *line, int exit_status, char **env)
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
