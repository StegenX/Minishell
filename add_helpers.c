/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 08:32:57 by aagharbi          #+#    #+#             */
/*   Updated: 2025/07/09 11:37:41 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_built_in(char *command)
{
	if (!command)
		return (0);
	if (ft_strnstr(command, "echo", 4))
		return (1);
	else if (ft_strnstr(command, "cd", 2))
		return (1);
	else if (ft_strnstr(command, "export", 6))
		return (1);
	else if (ft_strnstr(command, "env", 3))
		return (1);
	else if (ft_strnstr(command, "exit", 4))
		return (1);
	else if (ft_strnstr(command, "unset", 5))
		return (1);
	return (0);
}

void	free_list(char **list, char *new_list)
{
	int	i;

	i = 0;
	if (list)
	{
		while (list[i])
		{
			free(list[i]);
			i++;
		}
		free(list);
		list = NULL;
	}
	if (new_list)
		free(new_list);
}

int	one_string(const char *str, char c)
{
	int	i;
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		if (str[i] == c && (!in_double && !in_single))
			return (i);
		i++;
	}
	return (i);
}

char	*get_env_value(const char *var, char **env)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1, NULL));
		i++;
	}
	return (NULL);
}

void	set_here_doc(t_flags *flags)
{
	int		tmp_fd;
	char	tmp[6];
	char	c;
	int		i;

	i = 0;
	flags->tmp_path = "/tmp/here_doc_";
	tmp_fd = open("/dev/random", O_RDONLY);
	while (i < 5)
	{
		read(tmp_fd, &c, 1);
		if (c > 31 && c < 127 && c != '/')
			tmp[i++] = c;
	}
	tmp[i] = '\0';
	if (tmp_fd < 0)
	{
		write(2, "minishell: open failed\n", 24);
		make_input_clear(flags);
	}
	ft_close(&tmp_fd);
	flags->tmp_path = ft_strjoin(flags->tmp_path, tmp, flags);
}
