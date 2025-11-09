/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:52:33 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 14:57:07 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal;

int	flaging(int flag)
{
	static int	signal;

	if (flag == 4)
		return (signal);
	signal = flag;
	return (0);
}

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		if (flaging(4) == 3)
		{
			write(1, "\n", 1);
			_ft_exit(NULL, 130);
		}
		else if (flaging(4) == 1 || flaging(4) == 2)
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
			g_signal = SIGINT;
		}
	}
}

void	_ft_exit(t_flags *flags, int signal)
{
	static t_list	*head;
	static t_flags	*head_flags;

	if (flags)
	{
		head = flags->garbage_collector;
		head_flags = flags;
	}
	else
		ft_quit(head_flags, signal);
}

void	display_prompt(t_flags *flags, char **prompt)
{
	set_to_default(flags, 0);
	*prompt = readline("\001\033[1;36m\002minishell$ \001\033[0m\002");
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		flags->exit_status = 130;
	}
	if (!*prompt)
	{
		printf("exit\n");
		ft_quit(flags, flags->exit_status);
	}
}

int	main(int ac, char **av, char **env)
{
	t_flags	flags;

	ac = 0;
	av[0] = NULL;
	flags.garbage_collector = NULL;
	flags.env = init_env(env);
	flags.exit_status = 0;
	flags.valid = NULL;
	flags.path = NULL;
	flags.built_in_fork = 0;
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	ft_pwd(0, &flags);
	_ft_exit(&flags, 0);
	while (1)
	{
		if (make_input_clear(&flags) == 2)
			flags.exit_status = 2;
	}
}
