/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:42:04 by mkhandaq          #+#    #+#             */
/*   Updated: 2025/08/13 17:37:14 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*p;

	p = s;
	while (n--)
	{
		*p = '\0';
		p++;
	}
}
/*
int main(void)
{
	char p[10] = "1234567";
	bzero(NULL, 5);
	ft_bzero(NULL, 5);
}*/
