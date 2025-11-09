/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagharbi <aagharbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:22:09 by zael-mou          #+#    #+#             */
/*   Updated: 2025/06/27 08:32:51 by aagharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_pipes(char *prompt, t_flags *flags)
{
	int		i;
	char	c;

	i = 0;
	while (prompt[i])
	{
		if (prompt[i] == '\'' || prompt[i] == '"')
		{
			c = prompt[i];
			i++;
			while (prompt[i] != c && prompt[i])
				i++;
			i++;
		}
		else if (prompt[i] == '|')
		{
			flags->pipe++;
			i++;
		}
		else
			i++;
	}
	if (flags->pipe > -1)
		flags->pipe++;
	return (flags->pipe);
}

int	find_variable(char *prompt)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (prompt[i])
	{
		if (prompt[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	pipes_no_split(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] && str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
				i++;
		}
		if (str[i] && str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
		}
		if (str[i] && str[i] == '|' && str[i + 1] == '|')
			return (1);
		i++;
	}
	return (0);
}

int	check_syntax(char **splited_prompt, int *i)
{
	if (pipes_no_split(splited_prompt[*i]))
		return (1);
	if (splited_prompt[0][0] == '|')
		return (1);
	if (splited_prompt[*i][0] == '|' && splited_prompt[*i][1] == '|')
		return (1);
	if (splited_prompt[*i][0] == '|')
	{
		if (splited_prompt[*i][1] == '\0' && (splited_prompt[*i + 1] == NULL
				|| splited_prompt[*i + 1][0] == '|'))
			return (1);
	}
	if (splited_prompt[*i][0] == '|' && (splited_prompt[*i - 1][0] == '<'
			|| splited_prompt[*i - 1][0] == '>'))
		return (1);
	return (0);
}

int	pipes_syntax(char *prompt, t_flags *flags)
{
	int		i;
	char	**splited_prompt;

	i = 0;
	splited_prompt = ft_split(prompt, ' ');
	add_array(flags, splited_prompt);
	while (splited_prompt[i])
	{
		if (check_syntax(splited_prompt, &i))
			return (1);
		i++;
	}
	return (0);
}
