/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putun.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:01:11 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/02/07 16:43:41 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int	ft_putun(unsigned int n)
{
	char	c;
	int		tlen;

	tlen = 0;
	if (n >= 10)
		tlen += ft_putun(n / 10);
	c = n % 10 + '0';
	tlen += write(1, &c, 1);
	return (tlen);
}
