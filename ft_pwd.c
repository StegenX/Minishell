/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagharbi <aagharbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 08:19:14 by aagharbi          #+#    #+#             */
/*   Updated: 2025/07/08 20:55:52 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define CD_MESSAGE "cd: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n"

int	regenerate_path(char *old_path)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (0);
	else
	{
		ft_memset(old_path, 0, 10000);
		ft_strlcpy(old_path, path, ft_strlen(path) + 1);
		free(path);
	}
	return (1);
}

void	print_path(t_flags *flags, char *path)
{
	if (path[0] != '\0')
		printf("%s\n", path);
	else
	{
		write(2, CD_MESSAGE, ft_strlen(CD_MESSAGE));
		if (flags->built_in_fork)
			ft_quit(flags, 1);
		flags->dont_get_pstatus = 1;
	}
}

void	cd_first_enter(t_flags *flags, char *path)
{
	if (regenerate_path(path) == 0)
		write(2, CD_MESSAGE, ft_strlen(CD_MESSAGE));
	else
		ft_export(ft_strjoin("export PWD=", path, flags), flags, 1, 1);
}

void	ft_pwd(int flag, t_flags *flags)
{
	static char	path[10000];

	if (flag == 2 || flag == 1)
	{
		if (flag == 2)
			print_path(flags, path);
		else
		{
			if (regenerate_path(path) == 1)
				ft_export(ft_strjoin("export PWD=", path, flags), flags, 1, 1);
			else
			{
				write(2, CD_MESSAGE, ft_strlen(CD_MESSAGE));
				if (flags->built_in_fork)
					ft_quit(flags, 1);
				flags->dont_get_pstatus = 1;
			}
		}
	}
	else if (flag == 0)
		cd_first_enter(flags, path);
}
