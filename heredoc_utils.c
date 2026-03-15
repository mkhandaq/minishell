/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:19:24 by aalemami          #+#    #+#             */
/*   Updated: 2026/03/15 09:19:26 by aalemami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
