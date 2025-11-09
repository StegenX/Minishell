/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 01:52:07 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 14:25:05 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_spaces(char *prompt)
{
	int	i;

	i = 0;
	while (prompt[i])
	{
		if (!is_white_space(prompt[i]))
			return (0);
		i++;
	}
	return (1);
}

void	get_exit_status(t_flags *flags)
{
	if (flags->dont_get_pstatus > -1)
		flags->exit_status = flags->dont_get_pstatus;
	else if (WIFEXITED(flags->status))
		flags->exit_status = WEXITSTATUS(flags->status);
	else if (WIFSIGNALED(flags->status))
	{
		flags->exit_status = 128 + WTERMSIG(flags->status);
		if (flags->exit_status == 131)
			write(2, "Quit (core dumped)\n", 19);
	}
	if (flags->cntrl_c_flag)
		write(1, "\n", 1);
	ft_free_all(flags);
}

int	if_valid_syntax(t_flags *flags, char **prompt)
{
	if (calcule_quotes(*prompt) % 2 == 1)
	{
		free(*prompt);
		write(2, "minishell: syntax error: unclosed quotes\n", 41);
		return (2);
	}
	*prompt = trim_invalid_syntax(*prompt, flags);
	if (!*prompt)
		return (2);
	if (pipes_syntax(*prompt, flags))
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		return (2);
	}
	return (0);
}

int	start_command(t_flags *flags, char *prompt)
{
	int	return_status;

	if (check_spaces(prompt))
		return (free(prompt), -1);
	if (*prompt)
	{
		add_history(prompt);
		if (if_valid_syntax(flags, &prompt))
			return (2);
		return_status = start_business(prompt, flags);
		if (!return_status)
			return (2);
		else if (return_status == 3)
			return (1);
		get_exit_status(flags);
	}
	return (-1);
}

int	make_input_clear(t_flags *flags)
{
	char	*prompt;
	int		return_status;

	prompt = NULL;
	while (1)
	{
		flaging(1);
		display_prompt(flags, &prompt);
		return_status = start_command(flags, prompt);
		if (return_status > -1)
			return (return_status);
	}
}
