/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:10:17 by aagharbi          #+#    #+#             */
/*   Updated: 2025/07/05 14:27:20 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	ft_atoll(const char *nptr)
{
	long	nb;
	int		sign;

	nb = 0;
	sign = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr++ == '-')
			sign = -1;
	}
	while (*nptr >= 48 && *nptr <= 57)
	{
		nb = nb * 10 + (*nptr - 48);
		nptr++;
	}
	return (nb * sign);
}

char	*ft_strjoinp(const char *s1, const char *s2, char c, t_flags *flags)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*strings_cat;
	char	*head;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	strings_cat = ft_malloc(s1_len + s2_len + 2, flags);
	if (strings_cat == NULL)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	strings_cat[s1_len] = c;
	if (strings_cat == NULL)
		return (NULL);
	head = strings_cat;
	while (*s1)
		*strings_cat++ = *s1++;
	strings_cat++;
	while (*s2)
		*strings_cat++ = *s2++;
	*strings_cat = '\0';
	return (head);
}

void	which_qutes(char *str, t_flags *flags)
{
	int	i;

	flags->quotes = '"';
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			flags->quotes = str[i];
			break ;
		}
		i++;
	}
}

void	malloc_failed(const char *str, int line, t_flags *flags, int ps)
{
	if (ps)
	{
		write(2, "minishell: ", 12);
		write(2, "child process: malloc failed at ", 34);
		write(2, str, ft_strlen(str));
		write(2, ": line ", 7);
		ft_putnbr_fd(line, 2);
		write(2, "\n", 1);
	}
	else
	{
		write(2, "minishell: ", 12);
		write(2, "parent process: malloc failed at ", 34);
		write(2, str, ft_strlen(str));
		write(2, ": line ", 7);
		ft_putnbr_fd(line, 2);
		write(2, "\n", 1);
	}
	ft_quit(flags, 1);
}
