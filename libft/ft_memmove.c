/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:35:04 by mkhandaq          #+#    #+#             */
/*   Updated: 2025/08/25 17:29:58 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (d == s || n == 0)
		return (dest);
	if (d < s)
		ft_memcpy(dest, src, n);
	else if (s < d)
	{
		while (n > 0)
		{
			n--;
			d[n] = s[n];
		}
	}
	return (dest);
}
/*
int main()
{
//char	s1[] = "abcdef";
//char	s2[] = "abcdef";
//ft_memmove(NULL, NULL, 0);  // Overlapping!
printf("hello%s",(char *)memmove("","", 10));
//printf("%s",(char *)ft_memmove("","", 10));
//ft_memmove(NULL, NULL, 10);
//printf("%s",memmove(NULL, NULL, 10));
//printf("%s",memmove(NULL, NULL, 10));
//memmove(s2+9, s2 + 1, 5);
//printf("%c",'\n');
//printf("%s",s2);
}*/
