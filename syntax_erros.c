/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_erroes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:40:52 by marvin            #+#    #+#             */
/*   Updated: 2026/02/07 00:40:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_two_sided(t_token *node)
{
	t_token *tmp;

	tmp = node;
	if(is_two_sided(tmp))
	{
		ft_printf("shellGuys: syntax error near unexpected token `%s'\n"
			,tmp->value);
		return (0);
	}
	while(tmp && tmp->next)
	{
		if(is_two_sided(tmp) && is_two_sided(tmp->next))
		{
			ft_printf("shellGuys: syntax error near unexpected token '%s'\n"
						,tmp->next->value);
					return (0);
		}
		tmp = tmp->next;
	}
	if(is_two_sided(tmp))
	{
		ft_printf("shellGuys: syntax error near unexpected token `newline'\n");
		return (0);
	}
	return (1);
}

static int	check_one_sided(t_token *node)
{
	t_token *tmp;

	tmp = node;
	if (tmp->type == TOK_HEREDOC && is_sign(tmp->next))
	{
	    ft_printf("shellGuys: syntax error near unexpected token '%s'\n",
	              tmp->next->value);
	    return (0);
	}

	while(tmp && tmp->next)
	{
		if(is_one_sided(tmp) && is_sign(tmp->next))
		{
			ft_printf("shellGuys: syntax error near unexpected token '%s'\n"
						,tmp->next->value);
					return (0);
		}
		tmp = tmp->next;
	}
	if(is_one_sided(tmp))
	{
		ft_printf("shellGuys: syntax error near unexpected token `newline'\n");
		return (0);
	}
	return (1);
}

int	check_syntax_errors(t_token *node)
{
	if (!check_two_sided(node) || !check_one_sided(node))
		return (0);
	return (1);
}
