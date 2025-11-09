/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:06:04 by aagharbi          #+#    #+#             */
/*   Updated: 2025/06/26 15:40:48 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_changer(t_flags *flags, char *new_var)
{
	if (is_exist(flags->env, new_var))
		update_env(flags->env, new_var);
	else
		flags->env = ft_add_env(flags->env, new_var, flags);
}

int	valid_variable(char *str)
{
	int	i;

	i = 0;
	while ((str[i] && ft_isalnum(str[i])) || str[i] == '_')
		i++;
	return (i);
}

int	is_valid(char *var)
{
	int	i;

	i = 0;
	while (var[i] && var[i] != '=')
	{
		if (i == 0 && !ft_isalpha(var[i]) && var[i] != '_')
			return (0);
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	equal_sign(char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	is_exist(char **env, char *var)
{
	int	i;
	int	j;
	int	var_len;
	int	env_len;

	i = 0;
	j = 0;
	var_len = valid_variable(var);
	while (env[i])
	{
		env_len = valid_variable(env[i]);
		if (ft_strncmp(env[i], var, valid_variable(var)) == 0
			&& env_len == var_len)
			return (1);
		i++;
	}
	return (0);
}
