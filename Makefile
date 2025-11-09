NAME = minishell

FLAGS = -Wall -Wextra -Werror

SRC = minishell.c utils_two.c built-in.c helpers.c \
	parse_redirection.c parse_utils.c utils.c command_parsing.c \
	variable_expand.c make_input_beautiful.c built_in_helpers.c \
	ft_export.c ft_unset.c check_command.c ft_exit.c ft_echo.c \
	libft_functions.c ft_split.c libft_functions2.c libft_functions3.c \
	ft_second_split.c ft_pwd.c commad_compilation.c \
	garbage_collector.c initialize_variable.c commands_routine.c \
	input_errors_handler.c pipe_command_routine.c here_doc.c main_routine.c \
	parse_redirection_helpers.c add_helpers.c 

OBJ = ${SRC:.c=.o}

all: $(NAME)

bonus: all

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -lreadline -o $(NAME)

%.o: %.c minishell.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

ra: all clean

.PHONY: all bonus clean fclean re
