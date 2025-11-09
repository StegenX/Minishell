/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 23:49:13 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 15:05:56 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	clean_input(t_flags *flags, int i)
{
	int	return_status;

	set_to_default(flags, 1);
	if (ambigous_redirect(flags->command_list[i], flags))
		return (3);
	return_status = get_inoutfile(flags->command_list[i], flags);
	if (flags->here_doc_signal)
	{
		flags->dont_get_pstatus = 130;
		return (0);
	}
	if (!return_status)
		return (0);
	return (return_status);
}

void	command_loop(t_flags *flags)
{
	int (i), (return_status);
	i = -1;
	while (flags->command_list[++i])
	{
		return_status = clean_input(flags, i);
		if (!return_status)
			break ;
		if (return_status == 3)
		{
			close_pipes(flags);
			continue ;
		}
		flags->command_list[i] = remove_redirection(flags->command_list[i],
				flags);
		if (!*flags->command_list[i])
			continue ;
		if (find_variable(flags->command_list[i]))
			flags->command_name = expand_variables(flags->command_list[i],
					flags);
		else
			flags->command_name = flags->command_list[i];
		if (!deal_with_command(flags))
			break ;
	}
}

void	deal_with_pipes(char *prompt, t_flags *flags)
{
	int	i;

	flags->pids = ft_malloc((flags->pipe + 1) * sizeof(int), flags);
	flags->in_pipe = 1;
	if (!flags->pids)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	set_pids(flags, 0);
	flags->command_list = ft_ssplit(prompt, '|');
	add_array(flags, flags->command_list);
	command_loop(flags);
	i = 0;
	while (i < flags->pids_index)
	{
		waitpid(flags->pids[i++], &flags->status, 0);
		if (WIFSIGNALED(flags->status) && WTERMSIG(flags->status) == SIGINT)
			flags->cntrl_c_flag = 1;
	}
	flaging(1);
}
