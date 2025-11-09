/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_routine.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:03:21 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 14:27:19 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	alloc_pids(t_flags *flags, char *prompt)
{
	flags->pids = ft_malloc((flags->pipe + 2) * sizeof(int), flags);
	if (!flags->pids)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	set_pids(flags, 1);
	return (get_inoutfile(prompt, flags));
}

static void	handle_wait(t_flags *flags)
{
	if (flags->pipe == -1)
		waitpid(flags->pids[0], &flags->status, 0);
	if (WIFSIGNALED(flags->status) && WTERMSIG(flags->status) == SIGINT)
		flags->cntrl_c_flag = 1;
}

int	start_business(char *prompt, t_flags *flags)
{
	int	return_status;

	if (find_pipes(prompt, flags) > -1)
		return (deal_with_pipes(prompt, flags), 1);
	if (ambigous_redirect(prompt, flags))
		return (flags->exit_status = 1, 3);
	return_status = alloc_pids(flags, prompt);
	if (!return_status)
		return (0);
	else if (return_status == 2 || return_status == 3)
		return (1);
	prompt = remove_redirection(prompt, flags);
	if (find_variable(prompt))
		flags->command_name = expand_variables(prompt, flags);
	else
		flags->command_name = prompt;
	if (!*prompt)
		return (1);
	return_status = deal_with_command(flags);
	if (!return_status)
		return (0);
	else if (return_status == 2)
		return (1);
	handle_wait(flags);
	return (1);
}

char	*trim_invalid_syntax(char *prompt, t_flags *flags)
{
	int		i;
	int		j;
	char	*trimmed_prompt;

	i = 0;
	j = 0;
	trimmed_prompt = ft_malloc(calcule_new_string(prompt) + 1, flags);
	if (!trimmed_prompt)
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	while (prompt[i])
	{
		if (fill_quotes(prompt, &i, trimmed_prompt, &j))
			continue ;
		else if (!fill_string(prompt, trimmed_prompt, &i, &j))
			trimmed_prompt[j++] = prompt[i++];
		if (j == -1)
			return (NULL);
	}
	trimmed_prompt[j] = '\0';
	return (free(prompt), trimmed_prompt);
}
