/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagharbi <aagharbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:21:59 by zael-mou          #+#    #+#             */
/*   Updated: 2025/06/27 08:35:47 by aagharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_quotes(t_flags *flags, char **result, int *i, int *j)
{
	if (flags->prompt[*i] == '\'' && !flags->in_double_quote)
	{
		flags->in_single_quote = !flags->in_single_quote;
		result[0][(*j)++] = flags->prompt[(*i)++];
		return (1);
	}
	else if (flags->prompt[*i] == '"' && !flags->in_single_quote)
	{
		flags->in_double_quote = !flags->in_double_quote;
		result[0][(*j)++] = flags->prompt[(*i)++];
		return (1);
	}
	else if (flags->prompt[*i] == '$' && (flags->prompt[*i + 1] != '"'
			&& flags->prompt[*i + 1] != '\'') && (!ft_isalpha(flags->prompt[*i
					+ 1]) && flags->prompt[*i + 1] != '_'))
		return ((*i)++, 2);
	else if (flags->prompt[*i] == '$' && (!ft_isalpha(flags->prompt[*i + 1])
			&& flags->prompt[*i + 1] != '_') && !flags->in_double_quote
		&& !flags->in_single_quote)
		return ((*i)++, 1);
	else if (flags->prompt[*i] == '$' && !flags->in_single_quote)
		return ((*i)++, 2);
	return (0);
}

void	alloc_variable(char *value, char **result, t_flags *flags,
		char *var_name)
{
	char	*temp;
	size_t	old_len;

	temp = NULL;
	old_len = 0;
	if (value)
	{
		temp = *result;
		if (*result)
			old_len = ft_strlen(*result);
		*result = ft_malloc(old_len + ft_strlen(value) + ft_strlen(flags->prompt
					+ (*flags->i)) + 1, flags);
		if (!*result)
			malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
		if (var_name[0] == '?')
			ft_strlcpy(*result, temp, old_len + 1);
		else
			ft_strlcpy(*result, temp, *flags->j + 1);
		if (flags->prompt + (*flags->i + 1))
			ft_strlcpy(*result + *flags->j, value, ft_strlen(value) + 1);
		*flags->j += ft_strlen(value);
		free(value);
	}
}

void	exit_code(char **result, t_flags *flags, int *i, int *j)
{
	flags->j = j;
	flags->i = i;
	alloc_variable(ft_itoa(flags->exit_status), result, flags, "?");
	(*i)++;
	flags->exit_status = 0;
}

int	replace_variable(t_flags *flags, int *i, int *j, char **result)
{
	int		start;
	char	*var_name;
	char	*value;

	var_name = NULL;
	value = NULL;
	if (ft_isalnum(flags->prompt[*i]) || flags->prompt[*i] == '_')
	{
		start = *i;
		while (ft_isalnum(flags->prompt[*i]) || flags->prompt[*i] == '_')
			(*i)++;
		var_name = ft_substr(flags->prompt + start, 0, *i - start);
		value = get_env_value(var_name, flags->env);
		flags->j = j;
		flags->i = i;
		alloc_variable(value, result, flags, var_name);
		free(var_name);
		return (1);
	}
	else if (flags->prompt[*i] == '?')
		return (exit_code(result, flags, i, j), 1);
	return (0);
}

char	*expand_variables(char *prompt, t_flags *flags)
{
	char	*result;
	int		i;
	int		j;
	int		checker;

	result = ft_memset(ft_malloc(ft_strlen(prompt) + 1, flags), 0,
			ft_strlen(prompt) + 1);
	if (!result)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	i = 0;
	j = 0;
	flags->prompt = prompt;
	while (flags->prompt[i])
	{
		checker = skip_quotes(flags, &result, &i, &j);
		if (checker == 2)
		{
			if (!replace_variable(flags, &i, &j, &result))
				result[j++] = '$';
		}
		else if (!checker)
			result[j++] = flags->prompt[i++];
	}
	result[j] = '\0';
	return (result);
}
