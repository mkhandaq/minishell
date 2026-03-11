/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 00:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/11 00:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

static int	match_pattern(const char *pat, const char *str);

static int	match_star(const char *pat, const char *str)
{
	while (*pat == '*')
		pat++;
	if (!*pat)
		return (1);
	while (*str)
	{
		if (match_pattern(pat, str))
			return (1);
		str++;
	}
	return (0);
}

static int	match_pattern(const char *pat, const char *str)
{
	if (!*pat)
		return (!*str);
	if (*pat == '*')
		return (match_star(pat, str));
	if (*str && *pat == *str)
		return (match_pattern(pat + 1, str + 1));
	return (0);
}

int	count_matches(DIR *dir, const char *pat)
{
	struct dirent	*entry;
	int				count;

	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' || pat[0] == '.')
			if (match_pattern(pat, entry->d_name))
				count++;
		entry = readdir(dir);
	}
	return (count);
}

void	fill_matches(DIR *dir, const char *pat, char **matches)
{
	struct dirent	*entry;
	int				i;

	i = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' || pat[0] == '.')
			if (match_pattern(pat, entry->d_name))
				matches[i++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	matches[i] = NULL;
}
