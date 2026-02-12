/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 19:42:55 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/02/11 19:28:31 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr_GNL(const char *s, int c)
{
	size_t	i;

	i = 0;
	if ((char)c == '\0')
		return ((char *)s + ft_strlen_GNL(s));
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i++;
	}
	if (c == '\0')
		return ((char *)(s + i));
	return (NULL);
}

char	*ft_strjoin_GNL(char *s1, char *s2)
{
	char	*final;
	size_t	s1len;
	size_t	s2len;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup_GNL(s2));
	if (!s2)
		return (ft_strdup_GNL(s1));
	s1len = ft_strlen_GNL(s1);
	s2len = ft_strlen_GNL(s2);
	final = malloc(s1len + s2len + 1);
	if (!final)
		return (NULL);
	ft_memcpy_GNL(final, s1, s1len);
	ft_memcpy_GNL(final + s1len, s2, s2len);
	final[s1len + s2len] = '\0';
	return (final);
}

char	*ft_strdup_GNL(const char *s)
{
	char	*ptr;
	size_t	len;

	len = ft_strlen_GNL(s) + 1;
	ptr = malloc(len);
	if (ptr == NULL)
		return (NULL);
	ft_memcpy_GNL(ptr, s, len);
	return (ptr);
}

size_t	ft_strlen_GNL(const char *s)
{
	size_t	count;

	count = 0;
	while (s[count] != '\0')
	{
		count++;
	}
	return (count);
}

char	*ft_substr_GNL(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	tlen;

	if (!s)
		return (NULL);
	tlen = ft_strlen_GNL(s);
	if (len > tlen - start)
		len = tlen - start;
	if (start >= tlen)
		return (ft_strdup_GNL(""));
	sub = malloc(len + 1);
	if (!sub)
		return (NULL);
	ft_memcpy_GNL(sub, s + start, len);
	sub[len] = '\0';
	return (sub);
}
