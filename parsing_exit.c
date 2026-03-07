/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 08:29:05 by ali_shell         #+#    #+#             */
/*   Updated: 2026/03/07 10:21:14 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_isnumeric(const char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_atoll(const char *str, long long *out)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (sign == 1 && (result > (LLONG_MAX - (*str - '0')) / 10))
			return (0);
		if (sign == -1 && (-result < (LLONG_MIN + (*str - '0')) / 10))
			return (0);
		result = result * 10 + (*str - '0');
		str++;
	}
	*out = result * sign;
	return (1);
}

static int	count_args(t_token *node)
{
	int	count;

	count = 0;
	node = node->next;
	while (node && !is_sign(node))
	{
		count++;
		node = node->next;
	}
	return (count);
}

int	builtin_exit(t_token *node)
{
	int			argc;
	long long	code;

	ft_putstr_fd("exit\n", 2);
	argc = count_args(node);
	if (argc == 0)
		exit(0);
	if (!ft_isnumeric(node->next->value)
		|| !ft_atoll(node->next->value, &code))
	{
		ft_putstr_fd("shellGuys: exit: ", 2);
		ft_putstr_fd(node->next->value, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	if (argc > 1)
	{
		ft_putstr_fd("shellGuys: exit: too many arguments\n", 2);
		return (1);
	}
	exit((unsigned char)code);
	return (0);
}