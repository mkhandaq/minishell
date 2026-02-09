/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 12:47:03 by mkhandaq          #+#    #+#             */
/*   Updated: 2025/09/03 20:51:56 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	words(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static size_t	wordslen(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0' && s[i] != c)
		i++;
	return (i);
}

static void	getfree(char **s, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(s[i]);
		i++;
	}
	free(s);
}

static int	split_loop(char const *s, char c, char **splited)
{
	size_t	j;
	size_t	len;

	j = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			len = wordslen(s, c);
			splited[j] = ft_substr(s, 0, len);
			if (!splited[j])
			{
				getfree(splited, j);
				return (0);
			}
			j++;
			s += len;
		}
	}
	splited[j] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**splited;

	if (!s)
		return (NULL);
	splited = malloc((words(s, c) + 1) * sizeof(char *));
	if (!splited)
		return (NULL);
	if (!split_loop(s, c, splited))
		return (NULL);
	return (splited);
}


/*char	**ft_split(char const *s, char c)
{
	size_t	j;
	char	**splited;

	if (!s)
		return (NULL);
	j = 0;
	splited = malloc((words(s, c) + 1) * sizeof(char *));
	if (!splited)
		return (NULL);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			splited[j] = ft_substr(s, 0, wordslen(s, c));
			if (!splited[j])
			{
				getfree(splited, j);
				return (NULL);
			}
			j++;
			s += wordslen(s, c);
		}
	}
	splited[j] = NULL;
	return (splited);
}*/
