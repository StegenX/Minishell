/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:32:40 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 11:52:27 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_flags
{
	int				infile;
	int				pipe;
	int				outfile;
	char			*infile_name;
	char			*outfile_name;
	char			**redirection_splited;
	char			*command_name;
	char			**env;
	char			**splited_env;
	char			*command_path;
	int				infile_fd;
	int				outfile_fd;
	char			**command_flags;
	char			*for_join;
	int				such_file;
	char			**command_list;
	int				fd[2];
	int				pipe_infile;
	int				status;
	int				append;
	int				here_doc;
	int				here_doc_signal;
	char			*here_doc_file;
	char			*tmp_path;
	char			*delimiter;
	int				dont_get_pstatus;
	int				here_doc_command;
	int				here_doc_fd;
	char			quotes;
	int				delimeter_quotes;
	char			*prompt;
	int				in_single_quote;
	int				in_double_quote;
	int				*j;
	int				*i;
	int				cntrl_c_flag;
	int				pids_index;
	pid_t			*pids;
	pid_t			heredoc_pid;
	struct stat		st;
	int				got_signle;
	int				exit_status;
	int				prv_in;
	int				prv_out;
	char			**valid;
	int				built_in_fork;
	int				execve_success;
	int				command_as_path;
	int				dont_change_it;
	t_list			*garbage_collector;
	int				find_variable;
	int				in_pipe;
	char			*path;
}					t_flags;

# ifndef RED
#  define RED "\033[1;31m"
# endif
# ifndef GREEN
#  define GREEN "\033[1;32m"
# endif
# ifndef BLUE
#  define BLUE "\033[1;34m"
# endif
# ifndef RESET
#  define RESET "\033[0m"
# endif

char				*ft_strjoinp(const char *s1, const char *s2, char c,
						t_flags *flags);
int					calcule_new_string(char *prompt);
int					find_equal(char *str);
int					which_built_in(char *command, t_flags *flags, int run_it);
int					find_char(char *str, char c);
char				new_quotes(char *str);
int					number_of_quotes(char *str, char c);
int					calcule_quotes(char *str);
char				*remove_redirection(char *prompt, t_flags *flags);
char				*remove_quotes(char *s, char c, t_flags *flags);
int					deal_with_command(t_flags *flags);
int					find_string(char *env, char *to_find);
void				run_command(t_flags *flags);
int					check_if_built_in(char *command);
int					find_pipes(char *prompt, t_flags *flags);
int					find_variable(char *prompt);
int					pipes_syntax(char *prompt, t_flags *flags);
int					ft_strp(const char *s1, const char *s2, size_t n);
int					find_string(char *env, char *to_find);
int					here_doc(t_flags *flags);
void				child_process(t_flags *flags);
void				fill_array(t_flags *flags, int *i);
void				set_to_default(t_flags *flags, int pipe);
int					get_inoutfile(char *prompt, t_flags *flag);
char				*get_env_value(const char *var, char **env);
char				*expand_variables(char *prompt, t_flags *flags);
void				malloc_failed(const char *str, int line, t_flags *flags,
						int ps);
char				*trim_invalid_syntax(char *prompt, t_flags *flags);
void				ft_export(char *export, t_flags *flags, int i, int flag);
void				ft_unset(char *unset, t_flags *flags);
void				ft_cd(char *cd, t_flags *flags);
void				ft_pwd(int flag, t_flags *flags);
void				ft_exit(char *_exit, t_flags *flags);
void				ft_env(char *env, t_flags *flags);
void				ft_echo(char *echo, t_flags *flags);
void				skip_space(char *export, int *i);
void				print_env(char **env, char **valid);
int					is_valid(char *var);
int					equal_sign(char *var);
int					is_exist(char **env, char *var);
void				update_env(char **env, char *var);
char				**ft_add_env(char **env, char *var, t_flags *flags);
int					valid_variable(char *str);
void				dup_fd(t_flags *flags);
void				reset_fd(int out, int in);
int					is_arg(char *arg, t_flags *flags);
char				**ft_remove_env(char **env, char *var, t_flags *flags);
long				ft_atoll(const char *nptr);
int					start_business(char *prompt, t_flags *flags);
int					make_input_clear(t_flags *flags);
void				deal_with_pipes(char *prompt, t_flags *flags);
int					ambigous_redirect(char *prompt, t_flags *flag);
int					is_white_space(unsigned int c);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
char				*ft_strjoin(const char *s1, const char *s2, t_flags *flags);
char				*ft_substr(const char *s, unsigned int start, size_t len);
char				*ft_strdup(const char *s, t_flags *flags);
int					ft_isalnum(int c);
void				ft_putstr_fd(char *s, int fd);
size_t				ft_strlen(const char *s);
void				ft_putchar_fd(char c, int fd);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
int					ft_isalpha(int c);
char				**ft_ssplit(char const *s, char c);
char				*ft_itoa(int n);
char				**ft_split(char const *s, char c);
void				which_qutes(char *str, t_flags *flags);
char				*delimiter_expand(char *prompt, t_flags *flags);
int					ft_isdigit(int c);
void				free_array(char **str);
int					flaging(int flag);
void				*ft_memset(void *b, int c, size_t len);
void				env_changer(t_flags *flags, char *new_var);
void				cd_helper(t_flags *flags);
char				*alloc_a_free_string(char *trimmed_prompt, char temp[2]);
int					check_syntax(char **splited_prompt, int *i);
void				free_string(char *str);
int					counter(char **str);
void				free_list(char **list, char *new_list);
void				*ft_malloc(size_t size, t_flags *flags);
void				add_array(t_flags *flags, char **array);
void				ft_free_all(t_flags *flags);
void				ft_quit(t_flags *flags, int exit_status);
char				**init_env(char **env);
void				set_pids(t_flags *flags, int pipes);
void				_ft_exit(t_flags *flags, int signal);
int					ambigous_redirect(char *prompt, t_flags *flag);
void				deal_with_pipes(char *prompt, t_flags *flags);
void				calcule_one(char **str, int *i, char c);
int					fill_quotes(char *prompt, int *i, char *trimmed_prompt,
						int *j);
int					fill_string(char *prompt, char *trimmed_prompt, int *i,
						int *j);
void				display_prompt(t_flags *flags, char **prompt);
void				start_here_doc(t_flags *flag, int i);
int					in_redirection(t_flags *flag, int i);
int					out_redirection(t_flags *flag, int i);
int					one_string(const char *str, char c);
void				set_here_doc(t_flags *flags);
void				close_pipes(t_flags *flags);
void				ft_putnbr_fd(int n, int fd);
void				handler(int sig);
void				ft_close(int *fd);
#endif
