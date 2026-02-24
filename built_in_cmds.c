/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 03:20:04 by marvin            #+#    #+#             */
/*   Updated: 2026/02/23 03:20:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_token **list)
{
	int i;

	i = 0;
	if ((*list)->next && !ft_strncmp((*list)->next->value, "-n", 2) 
		&& ft_strlen((*list)->next->value) == 2)
	{
		*list = (*list)->next;
		i = 1;
	}
	*list = (*list)->next;
	while(*list && !(is_sign(*list)))
	{
		ft_printf("%s ", (*list)->value);
		*list = (*list)->next;
	}
	if(!i)
		ft_printf("\n");
}

void	set_built_in_cmds(t_token **node)
{
	t_token *tmp;

	tmp = *node;
	while(tmp)
	{
		if (tmp && !ft_strncmp(tmp->value, "echo", 4) 
			&& ft_strlen(tmp->value) == 4)
			tmp->type = TOK_ECHO;
		else if (tmp && !ft_strncmp(tmp->value, "cd", 2) 
			&& ft_strlen(tmp->value) == 2)
			tmp->type = TOK_CD;
		else if (tmp && !ft_strncmp(tmp->value, "export", 6)
			&& ft_strlen(tmp->value) == 6)
			tmp->type = TOK_EXPORT;
		else if (tmp && !ft_strncmp(tmp->value, "unset", 5) 
			&& ft_strlen(tmp->value) == 5)
			tmp->type = TOK_UNSET;
		else if (tmp && !ft_strncmp(tmp->value, "env", 3) 
			&& ft_strlen(tmp->value) == 3)
			tmp->type = TOK_ENV;
		else if (tmp && !ft_strncmp(tmp->value, "pwd", 3) 
			&& ft_strlen(tmp->value) == 3)
			tmp->type = TOK_PWD;
		else if (tmp && !ft_strncmp(tmp->value, "exit", 4)
			&& ft_strlen(tmp->value) == 4)
			tmp->type = TOK_EXIT;
		if(is_one_sided(tmp))
			tmp = tmp->next;
		tmp = tmp->next;
	}
}
