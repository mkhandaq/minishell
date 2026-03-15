/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 09:17:03 by mkhandaq          #+#    #+#             */
/*   Updated: 2026/03/15 09:17:04 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	**env_copy;

	if (ac != 1 || !(av || *av))
		return (1);
	env_copy = dup_env(env);
	if (!env_copy)
		return (1);
	set_signals();
	shell_loop(env_copy);
}

// verify headers and make sure they are correct
