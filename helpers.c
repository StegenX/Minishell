/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:38:47 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/05 15:39:23 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_char(char *str, char c)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	new_quotes(char *str)
{
	char	c;

	c = '"';
	if (str == NULL || *str == '\0')
		return (c);
	while (*str)
	{
		if (*str == '\'' || *str == '"')
		{
			c = *str;
			break ;
		}
		str++;
	}
	return (c);
}

int	number_of_quotes(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (*str && *str == c)
			i++;
		str++;
		if (i % 2 == 0)
			c = new_quotes(str);
	}
	return (i);
}

void	calcule_one(char **str, int *i, char c)
{
	(*i)++;
	(*str)++;
	while (**str && **str != c)
		(*str)++;
	if (**str == c)
		(*i)++;
}

int	counter(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}
