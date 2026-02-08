/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:02:31 by mkhandaq          #+#    #+#             */
/*   Updated: 2025/08/16 17:02:34 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*templ;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		templ = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = templ;
	}
	*lst = NULL;
}
