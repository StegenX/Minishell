/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_input_beautiful.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:18:33 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/05 06:25:25 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	callcule_quotes_length(char *prompt, int *i, int *new_length)
{
	char	c;

	c = prompt[*i];
	if (prompt[*i] == '\'' || prompt[*i] == '"')
	{
		(*i)++;
		(*new_length)++;
		while (prompt[*i] && prompt[*i] != c)
		{
			(*i)++;
			(*new_length)++;
		}
		if (prompt[*i])
		{
			(*i)++;
			(*new_length)++;
		}
	}
	else
		return (0);
	return (1);
}

int	calcule_chars_length(char *prompt, int *i, int *new_length)
{
	char	c;

	c = prompt[*i];
	if (prompt[*i] == '>' || prompt[*i] == '<' || prompt[*i] == '|')
	{
		(*new_length) += 3;
		(*i)++;
		while (prompt[*i] && prompt[*i] == c)
		{
			(*new_length)++;
			(*i)++;
		}
		return (1);
	}
	return (0);
}

int	calcule_new_string(char *prompt)
{
	int	i;
	int	new_length;

	i = 0;
	new_length = 0;
	while (prompt[i])
	{
		if (!callcule_quotes_length(prompt, &i, &new_length))
		{
			if (!calcule_chars_length(prompt, &i, &new_length))
			{
				new_length++;
				i++;
			}
		}
	}
	return (new_length);
}

int	fill_quotes(char *prompt, int *i, char *trimmed_prompt, int *j)
{
	char	c;

	if (prompt[*i] == '\'' || prompt[*i] == '"')
	{
		c = prompt[*i];
		trimmed_prompt[(*j)++] = c;
		(*i)++;
		while (prompt[*i] && prompt[*i] != c)
			trimmed_prompt[(*j)++] = prompt[(*i)++];
		if (prompt[*i])
		{
			trimmed_prompt[(*j)++] = c;
			(*i)++;
		}
		return (1);
	}
	return (0);
}

int	fill_string(char *prompt, char *trimmed_prompt, int *i, int *j)
{
	char	c;
	int		count_chars;

	count_chars = 0;
	if (prompt[*i] == '>' || prompt[*i] == '<' || prompt[*i] == '|')
	{
		c = prompt[(*i)++];
		trimmed_prompt[(*j)++] = ' ';
		trimmed_prompt[(*j)++] = c;
		while (prompt[*i] && prompt[*i] == c)
		{
			count_chars++;
			trimmed_prompt[(*j)++] = c;
			(*i)++;
		}
		if (count_chars > 1)
		{
			write(2, "minishell: syntax error near unexpected token `", 47);
			write(2, &c, 1);
			return (write(2, "'\n", 2), *j = -1, 1);
		}
		trimmed_prompt[(*j)++] = ' ';
		return (1);
	}
	return (0);
}
