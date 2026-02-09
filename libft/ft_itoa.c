/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:05:59 by mkhandaq          #+#    #+#             */
/*   Updated: 2025/08/13 19:06:02 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>

static size_t	len(long n)
{
	size_t	le;

	le = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		n = -n;
	while (n > 0)
	{
		n = n / 10;
		le++;
	}
	return (le);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	lenn;
	long	num;

	num = n;
	lenn = len(num);
	if (num < 0)
		lenn++;
	str = malloc(lenn + 1);
	if (!str)
		return (NULL);
	str[lenn] = '\0';
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	if (num == 0)
		str[0] = '0';
	while (num > 0)
	{
		str[--lenn] = (num % 10) + '0';
		num = num / 10;
	}
	return (str);
}
