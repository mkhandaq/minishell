/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali_shell <ali_shell@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 00:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/07 10:21:19 by ali_shell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	sort_env(char **arr, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strncmp(arr[i], arr[j],
					ft_strlen(arr[i]) + ft_strlen(arr[j])) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_line(char *entry)
{
	int	i;

	ft_putstr_fd("declare -x ", 1);
	i = 0;
	while (entry[i] && entry[i] != '=')
		i++;
	write(1, entry, i);
	if (entry[i] == '=')
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(entry + i + 1, 1);
		ft_putstr_fd("\"\n", 1);
	}
	else
		ft_putstr_fd("\n", 1);
}

void	print_export(char **env)
{
	int		size;
	int		i;
	char	**sorted;

	size = 0;
	while (env[size])
		size++;
	sorted = malloc(sizeof(char *) * (size + 1));
	if (!sorted)
		return ;
	i = -1;
	while (++i < size)
		sorted[i] = env[i];
	sorted[size] = NULL;
	sort_env(sorted, size);
	i = -1;
	while (++i < size)
		print_export_line(sorted[i]);
	free(sorted);
}
