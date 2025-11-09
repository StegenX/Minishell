/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_variable.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:53:43 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 14:16:10 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_env(char **env)
{
	int		i;
	char	**new_env;

	i = counter(env);
	new_env = malloc((i + 1) * 8);
	if (!new_env)
		malloc_failed(__func__, __LINE__, NULL, 0);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i], NULL);
		if (!new_env[i])
		{
			free(new_env);
			malloc_failed(__func__, __LINE__, NULL, 0);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	set_pids(t_flags *flags, int pipes)
{
	int	i;

	i = 0;
	if (pipes)
		while (i < flags->pipe + 2)
			flags->pids[i++] = 0;
	else
		while (i < flags->pipe + 1)
			flags->pids[i++] = 0;
}

void	set_pointer(t_flags *flags)
{
	flags->infile_name = NULL;
	flags->outfile_name = NULL;
	flags->command_name = NULL;
	flags->command_path = NULL;
	flags->delimiter = NULL;
	flags->here_doc_file = NULL;
	flags->redirection_splited = NULL;
	flags->command_flags = NULL;
	flags->splited_env = NULL;
	flags->for_join = NULL;
}

void	set_ints(t_flags *flags)
{
	flags->here_doc_signal = 0;
	flags->got_signle = 0;
	flags->cntrl_c_flag = 0;
	flags->execve_success = 0;
	flags->command_as_path = 0;
	flags->dont_get_pstatus = -1;
	flags->dont_change_it = 0;
	flags->delimeter_quotes = 0;
	flags->infile = 0;
	flags->outfile = 0;
	flags->infile_fd = 0;
	flags->outfile_fd = 0;
	flags->such_file = 0;
	flags->find_variable = 0;
	flags->status = 0;
	flags->append = 0;
	flags->in_double_quote = 0;
	flags->in_single_quote = 0;
	flags->here_doc = 0;
	flags->here_doc_command = 0;
	flags->here_doc_fd = 0;
	flags->fd[0] = -1;
	flags->fd[1] = -1;
	flags->prv_in = -1;
	flags->prv_out = -1;
}

void	set_to_default(t_flags *flags, int pipe)
{
	set_here_doc(flags);
	set_ints(flags);
	set_pointer(flags);
	if (!pipe)
	{
		flags->built_in_fork = 0;
		flags->pids_index = 0;
		flags->pipe_infile = 0;
		flags->pipe = -1;
		flags->in_pipe = 0;
		flags->command_list = NULL;
	}
}
