/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 10:56:19 by aagharbi          #+#    #+#             */
/*   Updated: 2025/07/09 14:01:35 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_quit(t_flags *flags, int exit_status)
{
	if (!flags->built_in_fork)
	{
		ft_close(&flags->prv_in);
		ft_close(&flags->prv_out);
	}
	ft_free_all(flags);
	free_list(flags->env, NULL);
	free_list(flags->valid, NULL);
	rl_clear_history();
	ft_close(&flags->pipe_infile);
	exit(exit_status);
}

int	is_num(char *var)
{
	int	i;

	i = 0;
	if (var[0] == '-')
		i++;
	while (var[i])
	{
		if (!ft_isdigit(var[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_long(char *var)
{
	int		i;
	char	*min;
	char	*max;

	i = 1;
	min = "-9223372036854775808";
	max = "9223372036854775807";
	if (var[0] == '-')
	{
		while (var[i] == '0')
			i++;
		if (ft_strlen(var + i) > 19 || (ft_strlen(var + i) == 19
				&& ft_strncmp(min, var, 19) == 0 && var[19] > '8'))
			return (0);
	}
	else
	{
		i = 0;
		while (var[i] == '0')
			i++;
		if (ft_strlen(var + i) > 19 || (ft_strlen(var + i) == 19
				&& ft_strncmp(max, var, 18) == 0 && var[18] > '7'))
			return (0);
	}
	return (1);
}

void	ft_exit(char *_exit, t_flags *flags)
{
	char	**splited;

	splited = ft_split(_exit, ' ');
	add_array(flags, splited);
	write(1, "exit\n", 5);
	if (splited[1] && (!is_num(splited[1]) || !is_long(splited[1])))
	{
		write(2, "minishell: exit: ", 17);
		write(2, splited[1], ft_strlen(splited[1]));
		write(2, ": numeric argument required\n", 28);
		ft_quit(flags, 2);
	}
	else if (counter(splited) > 2)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		if (flags->built_in_fork)
			ft_quit(flags, 1);
		flags->dont_get_pstatus = 1;
	}
	else if (splited[1])
		ft_quit(flags, (unsigned char)ft_atoll(splited[1]));
	else
		ft_quit(flags, flags->exit_status);
}

void	ft_close(int *fd)
{
	if (*fd > 0)
	{
		close(*fd);
		*fd = -1;
	}
}
