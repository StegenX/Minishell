/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:45:53 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 11:40:41 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_fd(t_flags *flags)
{
	if (flags->fd[1] > 0)
		ft_close(&flags->fd[1]);
	if (flags->fd[0] > 0)
		ft_close(&flags->fd[0]);
	if (!flags->command_name || !*flags->command_name)
	{
		if (flags->pipe_infile > 0)
			ft_close(&flags->pipe_infile);
		ft_quit(flags, 0);
	}
	if (flags->infile)
	{
		if (flags->infile_fd == -1)
			ft_quit(flags, 1);
		dup2(flags->infile_fd, 0);
	}
	else if (flags->here_doc)
	{
		ft_close(&flags->here_doc_fd);
		flags->here_doc_fd = open(flags->tmp_path, O_RDONLY);
		if (flags->here_doc_fd == -1)
			perror("open");
		dup2(flags->here_doc_fd, 0);
		ft_close(&flags->here_doc_fd);
	}
}

void	fill_array(t_flags *flags, int *i)
{
	char	*tmp;

	flags->splited_env = ft_ssplit(flags->env[*i], ':');
	add_array(flags, flags->splited_env);
	flags->command_flags = ft_split(flags->command_name, ' ');
	add_array(flags, flags->command_flags);
	*i = 0;
	while (flags->command_flags[*i])
	{
		which_qutes(flags->command_flags[*i], flags);
		if (find_char(flags->command_flags[*i], flags->quotes))
		{
			tmp = remove_quotes(flags->command_flags[*i], flags->quotes, flags);
			flags->command_flags[*i] = tmp;
		}
		(*i)++;
	}
	*i = 0;
	which_qutes(flags->command_name, flags);
}

void	search_for_path(t_flags *flags, int *i)
{
	flags->command_path = flags->command_name;
	flags->for_join = ft_strjoinp(flags->splited_env[(*i)++] + 5,
			flags->command_flags[0], '/', flags);
	while (flags->for_join)
	{
		if (!access(flags->for_join, F_OK))
		{
			if (!access(flags->for_join, X_OK))
				break ;
			else
			{
				if (!flags->dont_change_it)
					flags->command_path = flags->for_join;
				flags->dont_change_it = 1;
			}
		}
		flags->for_join = ft_strjoinp(flags->splited_env[(*i)++],
				flags->command_flags[0], '/', flags);
	}
}

int	deal_with_command(t_flags *flags)
{
	int	i;

	i = 0;
	while (flags->env[i] && !find_string(flags->env[i], "PATH="))
		i++;
	if (which_built_in(flags->command_name, flags, 0))
		return (run_command(flags), 1);
	if (!flags->env[i])
	{
		close_pipes(flags);
		if (flags->command_name[0] == '\0')
			return (1);
		flags->dont_get_pstatus = 127;
		write(2, "minishell: ", 11);
		write(2, flags->command_name, ft_strlen(flags->command_name));
		write(2, ": no such file or directory\n", 28);
		return (2);
	}
	fill_array(flags, &i);
	search_for_path(flags, &i);
	if (flags->for_join != NULL)
		flags->command_path = flags->for_join;
	if (find_char(flags->command_flags[0], '/'))
		flags->such_file = 1;
	return (run_command(flags), 1);
}
