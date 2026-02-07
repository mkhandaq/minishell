/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:58:42 by mkhandaq          #+#    #+#             */
/*   Updated: 2025/08/07 17:58:44 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	cpylen;
	size_t	srclen;

	srclen = ft_strlen(src);
	if (size > 0)
	{
		if (srclen >= size)
			cpylen = size - 1;
		else
			cpylen = srclen;
		ft_memcpy(dst, src, cpylen);
		dst[cpylen] = '\0';
	}
	return (srclen);
}
/*
int	main(void)
{
char dest[]="malek khandaqji";
const char src[]="mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm";
ft_strlcpy(dest, src, 5);
printf("%s",dest);
}*/
