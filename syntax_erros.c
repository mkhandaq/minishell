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

static int	is_closed(t_token *node)
{
	t_token	*tmp;
	int		count_open;

	tmp = node;
	count_open = 0;
	while(tmp && count_open >= 0)
	{
		if(tmp->type == TOK_OPENBRC)
			count_open++;
		if(tmp->type == TOK_CLOSEBRC)
			count_open--;
		tmp = tmp->next;
	}
	if(count_open)
		return (0);
	return (1);
}

static int	check_brackets(t_token *node)
{
	t_token *tmp;

	tmp = node;
	if(!is_closed(tmp))
	{
		ft_printf("shellGuys: CLOSE THAT BRACKET\n");
		return (0);
	}
	while(tmp && tmp->next)
	{
		if(tmp->type == TOK_CLOSEBRC || tmp->type == TOK_OPENBRC)
		{
			if(is_two_sided(tmp->next))
			{
				ft_printf("shellGuys: parse error near `%s'\n", tmp->next->value);
				return (0);
			}
			if(tmp->type == TOK_OPENBRC && tmp->next->type == TOK_CLOSEBRC)
			{
				ft_printf("shellGuys: EMPTY BRACKET\n");
				return (0);
			}
		}
		if(tmp->type == TOK_CLOSEBRC && tmp->next->type == TOK_OPENBRC)
		{
			ft_printf("shellGuys: parse error near `('\n");
				return (0);
		}
		if((tmp->type == TOK_CMD || tmp->type == TOK_KEYWORD)
			&& tmp->next->type == TOK_OPENBRC)
		{
			ft_printf("shellGuys: number expected\n");
				return (0);
		}			
		tmp = tmp->next;
	}
	return (1);
}

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
		if(is_two_sided(tmp) && tmp->next->type == TOK_CLOSEBRC )
		{
			ft_printf("parse error near `%s'\n", tmp->next->value);
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
	if (!check_two_sided(node) || !check_one_sided(node) || !check_brackets(node))
		return (0);
	return (1);
}
