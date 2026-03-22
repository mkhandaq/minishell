/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_erros.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:20:01 by aalemami          #+#    #+#             */
/*   Updated: 2026/03/15 09:20:01 by aalemami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_syntax_error(char *value)
{
	ft_putstr_fd("shellGuys: syntax error near unexpected token `", 2);
	ft_putstr_fd(value, 2);
	ft_putstr_fd("'\n", 2);
}

static int	check_two_sided_pair(t_token *tmp)
{
	if (is_two_sided(tmp) && is_two_sided(tmp->next))
	{
		print_syntax_error(tmp->next->value);
		return (0);
	}
	if (is_two_sided(tmp) && tmp->next->type == TOK_CLOSEBRC)
	{
		ft_putstr_fd("shellGuys: parse error near `", 2);
		ft_putstr_fd(tmp->next->value, 2);
		ft_putstr_fd("'\n", 2);
		return (0);
	}
	return (1);
}

static int	check_two_sided(t_token *node)
{
	t_token	*tmp;

	tmp = node;
	if (is_two_sided(tmp))
	{
		print_syntax_error(tmp->value);
		return (0);
	}
	while (tmp && tmp->next)
	{
		if (!check_two_sided_pair(tmp))
			return (0);
		tmp = tmp->next;
	}
	if (is_two_sided(tmp))
	{
		ft_putstr_fd("shellGuys: syntax error near unexpected token", 2);
		ft_putstr_fd(" `newline'\n", 2);
		return (0);
	}
	return (1);
}

static int	check_one_sided(t_token *node)
{
	t_token	*tmp;

	tmp = node;
	if (tmp && tmp->type == TOK_HEREDOC && is_sign(tmp->next))
	{
		print_syntax_error(tmp->next->value);
		return (0);
	}
	while (tmp && tmp->next)
	{
		if (is_one_sided(tmp) && is_sign(tmp->next))
		{
			print_syntax_error(tmp->next->value);
			return (0);
		}
		tmp = tmp->next;
	}
	if (is_one_sided(tmp))
	{
		ft_putstr_fd("shellGuys: syntax error near unexpected token", 2);
		ft_putstr_fd(" `newline'\n", 2);
		return (0);
	}
	return (1);
}

int	check_syntax_errors(t_token *node)
{
	if (!check_two_sided(node)
		|| !check_one_sided(node) || !check_brackets(node))
		return (0);
	return (1);
}
