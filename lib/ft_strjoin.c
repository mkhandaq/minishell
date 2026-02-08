/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 22:37:18 by mkhandaq          #+#    #+#             */
/*   Updated: 2025/08/11 22:37:20 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*final;
	size_t	s1len;
	size_t	s2len;

	if (!s1 || !s2)
		return (NULL);
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	final = malloc(s1len + s2len + 1);
	if (!final)
		return (NULL);
	ft_memcpy(final, s1, s1len);
	ft_memcpy(final + s1len, s2, s2len);
	final[s1len + s2len] = '\0';
	return (final);
}
