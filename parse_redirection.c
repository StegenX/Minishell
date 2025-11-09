/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:53:02 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 11:42:59 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *s, char c, t_flags *flags)
{
	char	*new_str;
	size_t	str_len;
	size_t	i;
	size_t	j;

	if (!s)
		return (NULL);
	new_str = ft_malloc(ft_strlen(s) + 1 - number_of_quotes(s, c), flags);
	if (!new_str)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	i = 0;
	j = 0;
	str_len = 0;
	while (s[i++])
	{
		if (s[i - 1] != c)
			new_str[j++] = s[i - 1];
		else
			str_len++;
		if (str_len % 2 == 0)
			c = new_quotes(s + i);
	}
	new_str[j] = '\0';
	return (new_str);
}

int	invalid_char(char c)
{
	if (((c >= 0 && c <= 31) || c == 127 || c == 38 || c == 42 || c == 59
			|| c == 60 || c == 62 || c == 63 || c == 92 || c == 96 || c == 124
			|| c == 40 || c == 41 || c == 123 || c == 125))
	{
		return (1);
	}
	return (0);
}

int	check_if_valid(t_flags *flag, int i)
{
	if (!flag->redirection_splited[i])
	{
		write(2, "minishell: syntax error near "
			"unexpected token `newline'\n", 56);
		return (wait(NULL), 0);
	}
	else if (invalid_char(flag->redirection_splited[i][0]))
	{
		write(2, "minishell: syntax error near unexpected token `", 47);
		write(2, &flag->redirection_splited[i][0], 1);
		write(2, "'\n", 2);
		return (wait(NULL), 0);
	}
	return (1);
}

int	search_for_redirection(t_flags *flag, int i)
{
	int	return_status;

	if (!ft_strp(flag->redirection_splited[i - 1], "<", 1)
		|| !ft_strp(flag->redirection_splited[i - 1], "<<", 2)
		|| !ft_strp(flag->redirection_splited[i - 1], ">", 1)
		|| !ft_strp(flag->redirection_splited[i - 1], ">>", 2))
	{
		if (!check_if_valid(flag, i))
			return (flag->dont_get_pstatus = 2, 0);
		return_status = in_redirection(flag, i - 1);
		if (flag->here_doc)
			ft_close(&flag->here_doc_fd);
		if (!return_status)
		{
			if (!out_redirection(flag, i - 1))
				return (flag->exit_status = 1, 2);
		}
		else if (return_status == 2)
			return (3);
	}
	return (1);
}

int	get_inoutfile(char *prompt, t_flags *flag)
{
	int	i;
	int	return_status;

	i = 0;
	return_status = 0;
	flag->redirection_splited = ft_split(prompt, ' ');
	add_array(flag, flag->redirection_splited);
	while (flag->redirection_splited[i++] != NULL)
	{
		return_status = search_for_redirection(flag, i);
		if (!return_status)
			return (0);
		if (return_status == 2 || return_status == 3)
			return (return_status);
	}
	if (flag->here_doc)
		waitpid(flag->heredoc_pid, &flag->status, 0);
	if (WEXITSTATUS(flag->status) == 130 || flag->here_doc_signal)
	{
		flag->here_doc_signal = 1;
		flag->dont_get_pstatus = 130;
		return (3);
	}
	return (1);
}
