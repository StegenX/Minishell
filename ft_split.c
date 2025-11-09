/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagharbi <aagharbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 13:37:14 by zael-mou          #+#    #+#             */
/*   Updated: 2025/06/27 08:37:31 by aagharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strchr_for_white_spaces(const char *s, int c)
{
	int	i;
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '"' && !in_single)
			in_double = !in_double;
		else if (s[i] == '\'' && !in_double)
			in_single = !in_single;
		if ((s[i] == (unsigned char)c || is_white_space(s[i])) && (!in_double
				&& !in_single))
			return ((char *)s + i);
		i++;
	}
	if (s[i] == (unsigned char)c || is_white_space(s[i]))
		return ((char *)s + i);
	return (NULL);
}

static int	count_words(char *str, char c)
{
	int (count), (flag);
	count = 0;
	flag = 0;
	while (*str)
	{
		if (*str == '"' || *str == '\'')
		{
			if (!flag)
			{
				flag = 1;
				count++;
			}
			str += one_string(str, c) - 1;
		}
		else if (*str != c && !is_white_space(*str) && !flag)
		{
			flag = 1;
			count++;
		}
		else if (*str == c || is_white_space(*str))
			flag = 0;
		str++;
	}
	return (count);
}

static void	free_split(char **lst)
{
	int	i;

	i = 0;
	if (!lst)
		return ;
	while (lst[i])
	{
		free(lst[i]);
		i++;
	}
	free(lst);
}

static void	fill(char *s, char c, char **lst)
{
	size_t	word_len;
	int		i;

	i = 0;
	word_len = 0;
	while (*s)
	{
		if (*s && (*s != c && !is_white_space(*s)))
		{
			word_len = ft_strlen(s);
			if (ft_strchr_for_white_spaces(s, c))
				word_len = ft_strchr_for_white_spaces(s, c) - s;
			lst[i] = ft_substr(s, 0, word_len);
			if (!lst[i])
			{
				free_split(lst);
				return ;
			}
			s += word_len;
			i++;
		}
		else
			s++;
	}
	lst[i] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**lst;
	int		count;

	if (!s)
		return (NULL);
	count = count_words((char *)s, c) + 1;
	lst = malloc(count * sizeof(char *));
	if (!lst)
		return (NULL);
	fill((char *)s, c, lst);
	return (lst);
}
