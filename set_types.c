/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_types.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 02:31:08 by marvin            #+#    #+#             */
/*   Updated: 2026/02/07 02:31:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	split_sign_node(t_token **list)
{
	t_token *tmp;
	t_token *tmp_ptr;
	t_token *new;

	tmp = *list;
	tmp_ptr = NULL;
	if(is_sign(tmp))
	{
		new = malloc(sizeof(t_token));
		if (!new)
			return ;
		if(return_sign_len(tmp) == 1 && ft_strlen(tmp->value) > 1)
			new->value = ft_strdup(tmp->value + 1);
		else if(return_sign_len(tmp) == 2 && ft_strlen(tmp->value) > 2)
			new->value = ft_strdup(tmp->value + 2);
		else
		{
			free(new);
			return ;
		}
		new->type = TOK_KEYWORD;
		if(tmp->next)
			tmp_ptr = tmp->next;
		tmp->next = new;
		new->next = tmp_ptr;
	}
}

static void	set_splited_nodes(char *root, char **str1, char **str2)
{
	int	i;
	int	len;

	i = 0;
	if(!root)
		return ;
	while(root[i] && !(ft_strchr("|&<>", root[i])))
		i++;
	*str2 = malloc(i + 1);
	ft_memcpy(*str2, root, i);
	(*str2)[i] = '\0';
	len = ft_strlen(root + i);
	*str1 = malloc(len + 1);
	if(!*str1)
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

static int	split_nonsign_node(t_token **list)
{
	t_token	*tmp;
	t_token	*tmp_ptr;
	t_token	*new;
	char	*node_value;

	tmp = *list;
	tmp_ptr = NULL;
	if(!is_sign(tmp))
	{
		if(ft_strchr(tmp->value, '|') || ft_strchr(tmp->value, '&')
			|| ft_strchr(tmp->value, '<') || ft_strchr(tmp->value, '>'))
		{
			new = malloc(sizeof(t_token));
			if (!new)
				return (0);
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
			if(tmp->next)
				tmp_ptr = tmp->next;
			tmp->next = new;
			new->next = tmp_ptr;
			return (1);
		}
	}
	return (0);
}

static void	split_node_loop(t_token **list)
{
	t_token *tmp;

	tmp = *list;
	while(tmp)
	{
		if (split_nonsign_node(&tmp))
			tmp = tmp->next;
		split_sign_node(&tmp);
		tmp = tmp->next;
	}
}

void	set_types(t_token **list)
{
	t_token *tmp;

	tmp = *list;
	while(tmp)
	{
		set_sign(&tmp);
		tmp = tmp->next;
	}
	tmp = *list;
	split_node_loop(&tmp);
	if(!check_syntax_errors(tmp))
		return ;
	while(tmp && tmp->next)
	{
		if(is_one_sided(tmp))
		{
			if(tmp->type == TOK_REDIR_IN)
				tmp->next->type = TOK_INFILE;
			else if (tmp->type == TOK_HEREDOC)
				tmp->next->type = TOK_LIMITER;
			else
				tmp->next->type = TOK_OUTFILE;
		}
		else if(is_two_sided(tmp))
			tmp->next->type = TOK_CMD;
		tmp = tmp->next;
	}
	tmp = *list;
	while (tmp && tmp->next)
	{
		if(tmp->type == TOK_CMD && !(is_sign(tmp->next)))
			tmp->next->type = TOK_KEYWORD;
		tmp = tmp->next;
	}
}
