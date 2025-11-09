/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 01:05:40 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 13:54:15 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define HERE_DOC_ERROR "minishell: warning: here-document at line 1 \
delimited by end-of-file (wanted `"

void	check_input(char *input, t_flags *flags)
{
	if (!input)
	{
		ft_close(&flags->here_doc_fd);
		write(2, HERE_DOC_ERROR, 78);
		write(2, flags->delimiter, ft_strlen(flags->delimiter));
		write(2, "')\n", 3);
		ft_quit(flags, 0);
	}
}

void	here_doc_loop(char *input, t_flags *flags)
{
	char	*tmp;

	while (ft_strp(input, flags->delimiter, ft_strlen(flags->delimiter)))
	{
		if (flags->delimeter_quotes)
		{
			tmp = input;
			input = expand_variables(input, flags);
			free(tmp);
		}
		ft_putstr_fd(input, flags->here_doc_fd);
		ft_putchar_fd('\n', flags->here_doc_fd);
		if (!flags->delimeter_quotes)
			free(input);
		input = readline("> ");
		if (!input)
		{
			ft_close(&flags->here_doc_fd);
			write(2, HERE_DOC_ERROR, 78);
			write(2, flags->delimiter, ft_strlen(flags->delimiter));
			write(2, "')\n", 3);
			ft_quit(flags, 0);
		}
	}
	free(input);
}

int	here_doc(t_flags *flags)
{
	char	*input;

	flaging(0);
	if (flags->here_doc_signal)
		return (flags->dont_get_pstatus = 130, 0);
	flags->heredoc_pid = fork();
	if (flags->heredoc_pid == 0)
	{
		flaging(3);
		if (flags->here_doc_signal)
			ft_quit(flags, 130);
		input = readline("> ");
		check_input(input, flags);
		here_doc_loop(input, flags);
		ft_close(&flags->here_doc_fd);
		ft_quit(flags, 0);
	}
	return (1);
}

void	start_here_doc(t_flags *flag, int i)
{
	if (flag->here_doc)
		waitpid(flag->heredoc_pid, &flag->status, 0);
	if (WEXITSTATUS(flag->status) == 130)
		flag->here_doc_signal = 1;
	if (!find_char(flag->redirection_splited[i + 1], '"')
		&& !find_char(flag->redirection_splited[i + 1], '\''))
		flag->delimeter_quotes = 1;
	flag->delimiter = remove_quotes(flag->redirection_splited[i + 1],
			new_quotes(flag->redirection_splited[i + 1]), flag);
	flag->here_doc = 1;
	flag->infile = 0;
	ft_close(&flag->here_doc_fd);
	flag->here_doc_fd = open(flag->tmp_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (flag->here_doc_fd == -1)
	{
		write(2, "minishell: open failed to create fd for here_doc\n", 49);
		flag->dont_get_pstatus = 1;
		return ;
	}
	here_doc(flag);
}
