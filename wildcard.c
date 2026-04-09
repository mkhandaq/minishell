/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:20:08 by aalemami          #+#    #+#             */
/*   Updated: 2026/04/09 13:44:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_matches(DIR *dir, const char *pat);
void	fill_matches(DIR *dir, const char *pat, char **matches);

static void	sort_matches(char **arr, int n)
{
	int		i;
	int		j;
	char	*tmp;

	i = 1;
	while (i < n)
	{
		tmp = arr[i];
		j = i - 1;
		while (j >= 0 && ft_strncmp(arr[j], tmp,
				ft_strlen(arr[j]) + ft_strlen(tmp)) > 0)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = tmp;
		i++;
	}
}

static char	**get_matches(const char *pat)
{
	DIR		*dir;
	char	**matches;
	int		count;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	count = count_matches(dir, pat);
	closedir(dir);
	if (!count)
		return (NULL);
	matches = malloc(sizeof(char *) * (count + 1));
	if (!matches)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (free(matches), NULL);
	fill_matches(dir, pat, matches);
	closedir(dir);
	sort_matches(matches, count);
	return (matches);
}

static void	insert_match_nodes(t_token *tok, char **matches, int count)
{
	t_token	*new;
	t_token	*last;
	int		i;

	last = tok;
	i = 1;
	while (i < count)
	{
		new = malloc(sizeof(t_token));
		if (!new)
			break ;
		new->value = matches[i];
		new->type = tok->type;
		new->strtype = TOK_STR;
		new->is_exuted = 0;
		new->next = last->next;
		last->next = new;
		last = new;
		i++;
	}
}

static void	expand_token_wildcard(t_token *tok)
{
	char	**matches;
	int		count;

	matches = get_matches(tok->value);
	if (!matches)
		return ;
	count = 0;
	while (matches[count])
		count++;
	free(tok->value);
	tok->value = matches[0];
	tok->strtype = TOK_STR;
	insert_match_nodes(tok, matches, count);
	free(matches);
}

void	expand_wildcards(t_token *list)
{
	while (list)
	{
		if (list->strtype != TOK_SING
			&& list->type != TOK_LIMITER
			&& list->type != TOK_INFILE
			&& list->type != TOK_OUTFILE
			&& list->value && ft_strchr(list->value, '*'))
			expand_token_wildcard(list);
		list = list->next;
	}
}
