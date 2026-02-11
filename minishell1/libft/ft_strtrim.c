/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 22:52:56 by mkhandaq          #+#    #+#             */
/*   Updated: 2025/08/11 22:52:57 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	size_t	blen;
	size_t	flen;
	size_t	filen;
	char	*copy;

	blen = 0;
	flen = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[flen] != '\0' && ft_strchr(set, s1[flen]))
		flen++;
	len = ft_strlen(s1);
	filen = len;
	while (len > flen && ft_strchr(set, s1[len - 1]))
	{
		len--;
		blen++;
	}
	copy = malloc(filen - flen - blen + 1);
	if (!copy)
		return (NULL);
	ft_memcpy(copy, s1 + flen, filen - blen - flen);
	copy[filen - blen - flen] = '\0';
	return (copy);
}
