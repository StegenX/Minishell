/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagharbi <aagharbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:41:50 by aagharbi          #+#    #+#             */
/*   Updated: 2025/06/26 17:41:51 by aagharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(const char *s, t_flags *flags)
{
	char	*str;
	int		i;

	i = 0;
	if (!flags)
		str = malloc(ft_strlen(s) + 1);
	else
		str = ft_malloc(ft_strlen(s) + 1, flags);
	if (!str)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*new_str;
	size_t	str_len;
	size_t	i;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strdup("", NULL));
	if (len > str_len - start)
		len = str_len - start;
	new_str = malloc(len + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len && s[start])
		new_str[i++] = s[start++];
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_strjoin(const char *s1, const char *s2, t_flags *flags)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*strings_cat;
	char	*head;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	strings_cat = ft_malloc(s1_len + s2_len + 1, flags);
	if (strings_cat == NULL)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	head = strings_cat;
	while (*s1 || *s2)
	{
		if (*s1)
			*strings_cat++ = *s1++;
		else if (*s2)
			*strings_cat++ = *s2++;
	}
	*strings_cat = '\0';
	return (head);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
