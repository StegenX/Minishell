/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:07:24 by aagharbi          #+#    #+#             */
/*   Updated: 2025/07/04 23:58:57 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unset(char *unset, t_flags *flags)
{
	int		i;
	char	**var;

	var = ft_split(unset, ' ');
	add_array(flags, var);
	i = 1;
	while (var[i])
	{
		if (is_valid(var[i]) && !equal_sign(var[i]))
		{
			if (is_exist(flags->env, var[i]))
				flags->env = ft_remove_env(flags->env, var[i], flags);
			else
				flags->valid = ft_remove_env(flags->valid, var[i], flags);
		}
		i++;
	}
	flags->exit_status = 0;
}

char	**ft_remove_env(char **env, char *var, t_flags *flags)
{
	char **(new_env);
	int (i), (len), (j);
	i = 0;
	len = 0;
	j = 0;
	while (env && env[len])
		len++;
	new_env = malloc((len + 1) * sizeof(char *));
	if (!new_env)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], var, valid_variable(var)) == 0)
			i++;
		else
		{
			new_env[j] = ft_strdup(env[i], NULL);
			i++;
			j++;
		}
	}
	new_env[j] = NULL;
	free_list(env, NULL);
	return (new_env);
}

int	is_arg(char *arg, t_flags *flags)
{
	int	i;

	i = 1;
	(void)flags;
	if (arg && arg[0] == '-')
	{
		while (arg[i])
		{
			if (arg[i] != 'n')
				return (1);
			i++;
		}
		return (0);
	}
	return (1);
}
