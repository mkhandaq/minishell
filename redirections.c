/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:17:23 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:17:23 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("shellGuys: %s: No such file or directory\n", file);
		return (0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

static int	redir_out(char *file, int flags)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		ft_printf("shellGuys: %s: No such file or directory\n", file);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

static int	redir_heredoc(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (0);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

int	redirections(t_token *list)
{
	while (list)
	{
		if (list->type == TOK_REDIR_IN && list->next)
			if (!redir_in(list->next->value))
				return (0);
		if (list->type == TOK_REDIR_OUT && list->next)
			if (!redir_out(list->next->value, O_WRONLY | O_CREAT | O_TRUNC))
				return (0);
		if (list->type == TOK_REDIR_APPEND && list->next)
			if (!redir_out(list->next->value, O_WRONLY | O_CREAT | O_APPEND))
				return (0);
		if (list->type == TOK_HEREDOC && list->next)
			if (!redir_heredoc(list->next->value))
				return (0);
		list = list->next;
	}
	return (1);
}
