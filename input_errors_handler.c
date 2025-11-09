/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:35:36 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/05 06:04:07 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_valid(t_flags *flag, char **splited, int i, char **env)
{
	char	*tmp;
	int		error;

	error = 0;
	if ((ft_strncmp(splited[i], "<", 1) == 0 || (ft_strncmp(splited[i], ">",
					1) == 0)) && ft_strlen(splited[i]) == 1)
	{
		tmp = expand_variables(splited[i + 1], flag);
		env = ft_split(tmp, ' ');
		if (flag->find_variable)
		{
			if (!env || !env[0])
				error = 1;
			else if (counter(env) > 1)
				error = 1;
		}
		free_list(env, NULL);
	}
	return (error);
}

int	ambigous_redirect(char *prompt, t_flags *flag)
{
	int		i;
	char	**splited;
	char	**env;

	i = 0;
	splited = ft_split(prompt, ' ');
	if (find_variable(prompt))
		flag->find_variable = 1;
	add_array(flag, splited);
	env = NULL;
	while (splited[i] && splited[i + 1])
	{
		if (if_valid(flag, splited, i, env))
		{
			write(2, "minishell: ", 11);
			write(2, splited[i + 1], ft_strlen(splited[i + 1]));
			write(2, ": ambiguous redirect\n", 21);
			flag->dont_get_pstatus = 0;
			return (1);
		}
		i++;
	}
	return (0);
}

int	calcule_quotes(char *str)
{
	int	i;

	while (*str)
	{
		i = 0;
		if (*str == '"')
		{
			calcule_one(&str, &i, *str);
			if (i % 2 == 1)
				return (1);
		}
		else if (*str == '\'')
		{
			calcule_one(&str, &i, *str);
			if (i % 2 == 1)
				return (1);
		}
		str++;
	}
	return (2);
}
