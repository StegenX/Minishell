/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 04:27:05 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 13:12:00 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	found_redirection(t_flags *flags, int *i)
{
	int	found;

	found = *i;
	if (!ft_strncmp(flags->redirection_splited[*i], "<", 1))
		*i += 2;
	else if (!ft_strncmp(flags->redirection_splited[*i], ">>", 2))
		*i += 2;
	else if (!ft_strncmp(flags->redirection_splited[*i], ">", 1))
		*i += 2;
	else if (!ft_strncmp(flags->redirection_splited[*i], "<<", 1))
		*i += 2;
	return (found - *i);
}

char	*remove_redirection(char *prompt, t_flags *flags)
{
	int		i;
	char	*tmp;

	i = 0;
	flags->redirection_splited = ft_split(prompt, ' ');
	add_array(flags, flags->redirection_splited);
	prompt = ft_strdup("", flags);
	while (flags->redirection_splited[i])
	{
		if (!found_redirection(flags, &i))
		{
			tmp = prompt;
			prompt = ft_strjoin(prompt, flags->redirection_splited[i], flags);
			if (flags->redirection_splited[i + 1])
			{
				tmp = prompt;
				prompt = ft_strjoin(prompt, " ", flags);
			}
			i++;
		}
	}
	return (prompt);
}

int	in_redirection(t_flags *flag, int i)
{
	if (ft_strp(flag->redirection_splited[i], "<", 1) == 0)
	{
		flag->infile_name = remove_quotes(flag->redirection_splited[i + 1],
				new_quotes(flag->redirection_splited[i + 1]), flag);
		if (find_variable(flag->redirection_splited[i + 1]))
		{
			flag->infile_name = remove_quotes(
					expand_variables(flag->redirection_splited[i + 1], flag),
					new_quotes(flag->redirection_splited[i + 1]), flag);
		}
		if (flag->infile_fd > 0)
			ft_close(&flag->infile_fd);
		flag->infile_fd = open(flag->infile_name, O_RDONLY, 0644);
		if (flag->infile_fd == -1)
		{
			if (!flag->in_pipe)
				flag->dont_get_pstatus = 1;
			write(2, "minishell: ", 12);
			return (perror(flag->infile_name), 1);
		}
		return (flag->infile = 1, 1);
	}
	else if (ft_strp(flag->redirection_splited[i], "<<", 2) == 0)
		return (start_here_doc(flag, i), 1);
	return (0);
}

static int	check_which_out_redirection(t_flags *flag, int i)
{
	if (flag->outfile_fd > 0)
		ft_close(&flag->outfile_fd);
	if (ft_strp(flag->redirection_splited[i], ">", 1) == 0)
	{
		flag->outfile_fd = open(flag->outfile_name,
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (flag->outfile_fd == -1)
		{
			write(2, "minishell: ", 12);
			flag->dont_get_pstatus = !flag->in_pipe;
			return (perror(flag->outfile_name), 1);
		}
	}
	else
	{
		flag->outfile_fd = open(flag->outfile_name,
				O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (flag->outfile_fd == -1)
		{
			write(2, "minishell: ", 12);
			flag->dont_get_pstatus = !flag->in_pipe;
			return (perror(flag->outfile_name), 1);
		}
	}
	return (1);
}

int	out_redirection(t_flags *flag, int i)
{
	flag->outfile_name = remove_quotes(flag->redirection_splited[i + 1],
			new_quotes(flag->redirection_splited[i + 1]), flag);
	if (find_variable(flag->redirection_splited[i + 1]))
		flag->outfile_name = remove_quotes(expand_variables
				(flag->redirection_splited[i + 1], flag),
				new_quotes(flag->redirection_splited[i + 1]), flag);
	if (!check_which_out_redirection(flag, i))
		return (0);
	flag->outfile = 1;
	return (1);
}
