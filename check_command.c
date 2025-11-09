/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:18:29 by aagharbi          #+#    #+#             */
/*   Updated: 2025/07/09 14:04:28 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_fd(int out, int in)
{
	dup2(out, 1);
	dup2(in, 0);
	ft_close(&out);
	ft_close(&in);
}

void	check_command(char *command, t_flags *flags, char *splited_command)
{
	if (flags->pipe > 0)
		dup2(flags->fd[1], 1);
	if (flags->pipe_infile > 0)
	{
		dup2(flags->pipe_infile, 0);
		close(flags->pipe_infile);
	}
	dup_fd(flags);
	if (flags->outfile)
		dup2(flags->outfile_fd, 1);
	if (!ft_strp(splited_command, "echo", 4))
		ft_echo(ft_strdup(command, flags), flags);
	else if (!ft_strp(splited_command, "cd", 2))
		ft_cd(remove_quotes(command, new_quotes(command), flags), flags);
	else if (!ft_strp(splited_command, "export", 6))
		ft_export(ft_strdup(command, flags), flags, 1, 0);
	else if (!ft_strp(splited_command, "env", 3))
		ft_env(remove_quotes(command, new_quotes(command), flags), flags);
	else if (!ft_strp(splited_command, "exit", 4))
		ft_exit(remove_quotes(command, new_quotes(command), flags), flags);
	else if (!ft_strp(splited_command, "unset", 5))
		ft_unset(remove_quotes(command, new_quotes(command), flags), flags);
	else if (!ft_strp(splited_command, "pwd", 3))
		ft_pwd(2, flags);
}

void	run_built_in(char *command, t_flags *flags, char *tmp, int run_it)
{
	if (run_it)
	{
		if (flags->built_in_fork)
		{
			flaging(0);
			flags->pids[flags->pids_index] = fork();
			if (flags->pids[flags->pids_index] == 0)
			{
				signal(SIGQUIT, SIG_DFL);
				flaging(2);
				check_command(command, flags, tmp);
				ft_quit(flags, flags->exit_status);
			}
			flags->pids_index++;
		}
		else
		{
			if (flags->prv_in < 0 || flags->prv_out < 0)
			{
				write(2, "minishell: dup failed\n", 22);
				flags->dont_get_pstatus = 1;
			}
			check_command(command, flags, tmp);
		}
	}
}

int	which_built_in(char *command, t_flags *flags, int run_it)
{
	char	**splited_command;
	char	*tmp;

	tmp = NULL;
	if (!command)
		return (0);
	splited_command = ft_split(command, ' ');
	add_array(flags, splited_command);
	tmp = remove_quotes(splited_command[0], new_quotes(splited_command[0]),
			flags);
	if (!ft_strp(tmp, "echo", 4) || !ft_strp(tmp, "cd", 2) || !ft_strp(tmp,
			"export", 6) || !ft_strp(tmp, "env", 3) || !ft_strp(tmp, "exit", 4)
		|| !ft_strp(tmp, "unset", 5) || !ft_strp(tmp, "pwd", 3))
	{
		if (!flags->built_in_fork && run_it)
		{
			flags->prv_out = dup(1);
			flags->prv_in = dup(0);
		}
		run_built_in(command, flags, tmp, run_it);
		return (1);
	}
	return (0);
}
