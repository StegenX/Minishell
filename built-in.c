/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:06:00 by aagharbi          #+#    #+#             */
/*   Updated: 2025/07/08 20:59:56 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(char *env, t_flags *flags)
{
	int	i;

	i = 3;
	while (env[i] == ' ')
		i++;
	if (!env[i])
	{
		i = 0;
		while (flags->env[i])
		{
			printf("%s\n", flags->env[i]);
			i++;
		}
	}
	flags->exit_status = 0;
}

int	check_permission(char *filepath, t_flags *flags)
{
	if (stat(filepath, &flags->st) == 0)
	{
		if (S_ISDIR(flags->st.st_mode))
		{
			if (!(flags->st.st_mode & S_IXUSR))
			{
				write(2, "minishell: cd: ", 15);
				write(2, filepath, ft_strlen(filepath));
				return (write(2, ": Permission denied\n", 20), 0);
			}
		}
		else
		{
			write(2, "minishell: cd: ", 15);
			write(2, filepath, ft_strlen(filepath));
			return (write(2, ": not a directory\n", 19), 0);
		}
	}
	else
		return (perror(ft_strjoin("minishell: ", filepath, flags)), 0);
	return (1);
}

void	cd_faild(t_flags *flags, char *path)
{
	write(2, "minishell: cd: ", 15);
	write(2, path, ft_strlen(path));
	write(2, ": No such file or directory\n", 28);
	if (flags->built_in_fork)
		ft_quit(flags, 1);
	else
		flags->dont_get_pstatus = 1;
}

void	ft_cd(char *cd, t_flags *flags)
{
	char	**splited;

	splited = ft_split(cd, ' ');
	add_array(flags, splited);
	if (splited[1])
	{
		if (!check_permission(splited[1], flags))
		{
			if (flags->built_in_fork)
				ft_quit(flags, 1);
			else
			{
				flags->dont_get_pstatus = 1;
				return ;
			}
		}
		else if (chdir(splited[1]) == -1)
			cd_faild(flags, splited[1]);
		else
			cd_helper(flags);
	}
}

void	cd_helper(t_flags *flags)
{
	char	*new_path;
	char	*path;

	path = get_env_value("PWD=", flags->env);
	new_path = getcwd(NULL, 0);
	if (new_path != NULL)
	{
		ft_export(ft_strjoin("export PWD=", new_path, flags), flags, 1, 1);
		if (path != NULL)
		{
			ft_export(ft_strjoin("export OLDPWD=", path, flags), flags, 1, 1);
			free(path);
		}
		else
			ft_export(ft_strjoin("export OLDPWD=", new_path, flags), flags, 1,
				1);
		free(new_path);
	}
	ft_pwd(1, flags);
	free(path);
	flags->exit_status = 0;
}
