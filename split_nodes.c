/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalemami <aalemami@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:19:56 by aalemami          #+#    #+#             */
/*   Updated: 2026/03/15 09:19:56 by aalemami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_splited_nodes(char *root, char **str1, char **str2)
{
	int	i;
	int	len;

	i = 0;
	if (!root)
		return ;
	while (root[i] && !(ft_strchr("|&<>()", root[i])))
		i++;
	*str2 = malloc(i + 1);
	ft_memcpy(*str2, root, i);
	(*str2)[i] = '\0';
	len = ft_strlen(root + i);
	*str1 = malloc(len + 1);
	if (!*str1)
	{
		free(*str2);
		*str2 = NULL;
		free(root);
		return ;
	}
	ft_memcpy(*str1, root + i, len);
	(*str1)[len] = '\0';
	free(root);
}

static t_token	*new_sign_node(t_token *tmp)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->strtype = TOK_STR;
	new->is_exuted = 0;
	new->next = NULL;
	if (return_sign_len(tmp) == 1 && ft_strlen(tmp->value) > 1)
		new->value = ft_strdup(tmp->value + 1);
	else if (return_sign_len(tmp) == 2 && ft_strlen(tmp->value) > 2)
		new->value = ft_strdup(tmp->value + 2);
	else
		return (free(new), NULL);
	if (!set_sign(&new))
		new->type = TOK_KEYWORD;
	return (new);
}

void	split_sign_node(t_token **list)
{
	t_token	*tmp;
	t_token	*tmp_ptr;
	t_token	*new;

	tmp = *list;
	tmp_ptr = NULL;
	if (!is_sign(tmp))
		return ;
	new = new_sign_node(tmp);
	if (!new)
		return ;
	if (tmp->next)
		tmp_ptr = tmp->next;
	tmp->next = new;
	new->next = tmp_ptr;
}

static int	build_split_node(t_token *tmp, t_token **new_out)
{
	t_token	*new;
	char	*node_value;

	new = malloc(sizeof(t_token));
	if (!new)
		return (0);
	new->strtype = TOK_STR;
	new->is_exuted = 0;
	new->next = NULL;
	node_value = ft_strdup(tmp->value);
	if (!node_value)
	{
		free(new);
		return (0);
	}
	free(tmp->value);
	set_splited_nodes(node_value, &(new->value), &(tmp->value));
	if (!new->value || !tmp->value)
	{
		free(new);
		return (0);
	}
	set_sign(&new);
	*new_out = new;
	return (1);
}

int	split_nonsign_node(t_token **list)
{
	t_token	*tmp;
	t_token	*tmp_ptr;
	t_token	*new;

	tmp = *list;
	tmp_ptr = NULL;
	if (is_sign(tmp) || tmp->strtype != TOK_STR || (!ft_strchr(tmp->value, '|')
			&& !ft_strchr(tmp->value, '&') && !ft_strchr(tmp->value, '<')
			&& !ft_strchr(tmp->value, '>') && !ft_strchr(tmp->value, '(')
			&& !ft_strchr(tmp->value, ')')))
		return (0);
	if (!build_split_node(tmp, &new))
		return (0);
	if (tmp->next)
		tmp_ptr = tmp->next;
	tmp->next = new;
	new->next = tmp_ptr;
	return (1);
}
