/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 04:11:14 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/02/07 16:43:35 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

static int	puthexl(unsigned long n)
{
	const char	*base;
	char		c;
	int			i;

	i = 0;
	c = 0;
	base = "0123456789abcdef";
	if (n >= 16)
		i += puthexl(n / 16);
	c = base[n % 16];
	i += write(1, &c, 1);
	return (i);
}

int	ft_putptr(void *ptr)
{
	unsigned long	add;
	int				i;

	if (ptr == NULL)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	add = (unsigned long)ptr;
	write(1, "0x", 2);
	i = 2;
	i += puthexl(add);
	return (i);
}
