/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commad_compilation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:29:11 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 13:54:32 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_pipe(t_flags *flags)
{
	if (flags->pipe > 0)
		dup2(flags->fd[1], 1);
	if (flags->pipe_infile > 0)
		dup2(flags->pipe_infile, 0);
}

void	close_pipes(t_flags *flags)
{
	if (flags->pipe > 0)
	{
		if (flags->pipe_infile > 0)
			ft_close(&flags->pipe_infile);
		flags->pipe_infile = flags->fd[0];
	}
	if (flags->pipe > 0 && flags->fd[1] > 0)
		ft_close(&flags->fd[1]);
	if (flags->pipe == 0 && flags->pipe_infile > 0)
		ft_close(&flags->pipe_infile);
	if (flags->pipe > 0)
		flags->pipe--;
}

void	print_error(t_flags *flags)
{
	if (access(flags->command_path, X_OK) && !access(flags->command_path, F_OK))
	{
		write(2, "minishell: ", 11);
		write(2, flags->command_path, ft_strlen(flags->command_path));
		write(2, ": Permission denied\n", 20);
		ft_quit(flags, 126);
	}
	else if (flags->such_file)
	{
		write(2, "minishell: ", 11);
		write(2, flags->command_flags[0], ft_strlen(flags->command_flags[0]));
		write(2, ": No such file or directory\n", 28);
	}
	else
	{
		if (flags->command_flags[0][0])
		{
			write(2, "minishell: ", 11);
			write(2, flags->command_flags[0],
				ft_strlen(flags->command_flags[0]));
			write(2, ": command not found\n", 20);
		}
		else
			write(2, "minishell: command not found\n", 29);
	}
}

void	child_process(t_flags *flags)
{
	dup_pipe(flags);
	dup_fd(flags);
	if (flags->outfile)
		dup2(flags->outfile_fd, 1);
	if (flags->infile_fd < 0 || flags->outfile_fd < 0)
		ft_quit(flags, 1);
	if (!*flags->command_flags)
		ft_quit(flags, 0);
	if (*flags->command_flags && flags->command_flags[0][0] == '/')
	{
		flags->command_path = flags->command_flags[0];
		flags->command_as_path = 1;
	}
	if (*flags->command_flags[0] && stat(flags->command_path, &flags->st) == 0
		&& S_ISDIR(flags->st.st_mode))
	{
		write(2, "minishell: ", 11);
		write(2, flags->command_name, ft_strlen(flags->command_name));
		write(2, ": is a directory\n", 17);
		ft_quit(flags, 126);
	}
	execve(flags->command_path, flags->command_flags, flags->env);
	print_error(flags);
	ft_close(&flags->pipe_infile);
	ft_quit(flags, 127);
}

void	run_command(t_flags *flags)
{
	if (flags->pipe > 0)
		pipe(flags->fd);
	if (flags->pipe > 0)
		flags->built_in_fork = 1;
	if (which_built_in(flags->command_name, flags, 1))
	{
		if (!flags->built_in_fork)
			reset_fd(flags->prv_out, flags->prv_in);
	}
	else
	{
		flaging(0);
		flags->pids[flags->pids_index] = fork();
		if (flags->pids[flags->pids_index] == 0)
		{
			signal(SIGQUIT, SIG_DFL);
			flaging(2);
			child_process(flags);
		}
		flags->pids_index++;
	}
	close_pipes(flags);
}
