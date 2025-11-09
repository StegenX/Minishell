/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:07:22 by aagharbi          #+#    #+#             */
/*   Updated: 2025/07/05 06:38:36 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**exporting_var(char **valid, char *var, t_flags *flags)
{
	static int	j = 0;
	int			i;
	char		**tmp;

	i = 0;
	if (j > 0)
		tmp = malloc((counter(valid) + 2) * sizeof(char *));
	else
		tmp = malloc(2 * sizeof(char *));
	if (!tmp)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	j += 1;
	if (valid && valid[i])
	{
		while (valid[i])
		{
			tmp[i] = ft_strdup(valid[i], NULL);
			i++;
		}
	}
	tmp[i] = ft_strdup(var, NULL);
	tmp[i + 1] = NULL;
	free_list(valid, NULL);
	return (tmp);
}

void	ft_export(char *export, t_flags *flags, int i, int flag)
{
	char **(var), *(new_var);
	var = ft_split(export, ' ');
	add_array(flags, var);
	if (counter(var) == 1)
	{
		print_env(flags->env, flags->valid);
		return ;
	}
	while (var[i++])
	{
		new_var = remove_quotes(var[i - 1], new_quotes(var[i - 1]), flags);
		if (is_valid(new_var) && equal_sign(new_var))
			env_changer(flags, new_var);
		else if (!is_valid(new_var))
		{
			write(2, "minishell: export: ", 19);
			write(2, new_var, ft_strlen(new_var));
			write(2, ": not a valid identifier\n", 25);
			flags->dont_get_pstatus = 1;
		}
		else
			flags->valid = exporting_var(flags->valid, var[i - 1], flags);
	}
	if (flags->built_in_fork && !flag)
		ft_quit(flags, flags->dont_get_pstatus);
}

char	**ft_add_env(char **env, char *var, t_flags *flags)
{
	int		i;
	int		len;
	char	**new_env;
	char	*tmp;

	tmp = NULL;
	i = 0;
	len = 0;
	while (env[len])
		len++;
	new_env = malloc((len + 2) * sizeof(char *));
	if (!new_env)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i], NULL);
		i++;
	}
	free_list(env, NULL);
	new_env[i] = ft_strdup(var, NULL);
	new_env[i + 1] = NULL;
	return (new_env);
}

void	update_env(char **env, char *var)
{
	int		i;
	int		var_len;
	int		env_len;
	char	*temp;

	i = 0;
	var_len = valid_variable(var);
	env_len = 0;
	temp = NULL;
	while (env[i])
	{
		env_len = valid_variable(env[i]);
		if (ft_strncmp(env[i], var, valid_variable(var)) == 0
			&& env_len == var_len)
		{
			temp = env[i];
			env[i] = ft_strdup(var, NULL);
			free(temp);
			return ;
		}
		i++;
	}
}

void	print_env(char **env, char **valid)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("declare -x %s\n", env[i]);
		i++;
	}
	i = 0;
	while (valid && valid[i])
	{
		printf("declare -x %s\n", valid[i]);
		i++;
	}
}
