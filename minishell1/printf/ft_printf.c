/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:04:40 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/02/07 16:43:07 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

static int	handler(char c, va_list ar)
{
	if (c == 'c')
		return (ft_putchar(va_arg(ar, int)));
	if (c == 's')
		return (ft_putstr(va_arg(ar, char *)));
	if (c == 'i' || c == 'd')
		return (ft_putnbr(va_arg(ar, int)));
	if (c == 'u')
		return (ft_putun(va_arg(ar, unsigned int)));
	if (c == 'x')
		return (ft_puthexl(va_arg(ar, unsigned int)));
	if (c == 'X')
		return (ft_puthexu(va_arg(ar, unsigned int)));
	if (c == 'p')
		return (ft_putptr(va_arg(ar, void *)));
	if (c == '%')
		return (ft_putchar('%'));
	else
		return (ft_putchar('%') + ft_putchar(c));
}

int	ft_printf(const char *arg, ...)
{
	va_list	ar;
	int		i;
	int		printc;

	printc = 0;
	if (!arg)
		return (-1);
	i = 0;
	va_start(ar, arg);
	while (arg[i])
	{
		if (arg[i] == '%' && arg[i + 1])
		{
			printc += handler(arg[i + 1], ar);
			i = i + 2;
		}
		else
			printc += ft_putchar(arg[i++]);
	}
	va_end(ar);
	return (printc);
}
