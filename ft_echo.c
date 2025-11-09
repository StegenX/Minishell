/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagharbi <aagharbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 10:51:01 by aagharbi          #+#    #+#             */
/*   Updated: 2025/07/02 05:34:55 by aagharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_helper(char **list, int *i, t_flags *flags)
{
	char	*new_list;

	while (list[*i])
	{
		new_list = remove_quotes(list[*i], new_quotes(list[*i]), flags);
		printf("%s", new_list);
		if (list[*i + 1])
			printf(" ");
		(*i)++;
	}
	printf("\n");
}

void	echo_helper_2(char **list, int *i, t_flags *flags)
{
	char	*new_list;

	while (!is_arg(list[*i], flags))
		(*i)++;
	while (list[*i])
	{
		new_list = remove_quotes(list[*i], new_quotes(list[*i]), flags);
		printf("%s", new_list);
		if (list[*i + 1])
			printf(" ");
		(*i)++;
	}
}

void	ft_echo(char *echo, t_flags *flags)
{
	int		i;
	char	**list;

	i = 1;
	list = ft_split(echo, ' ');
	add_array(flags, list);
	if (is_arg(list[i], flags))
	{
		echo_helper(list, &i, flags);
		return ;
	}
	else
	{
		echo_helper_2(list, &i, flags);
		return ;
	}
}
