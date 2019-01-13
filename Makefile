all:
	clang -Wall -Wextra -Werror -o ft_ls \
		ft.c \
		list.c \
		old.c \
		get_time.c \
		list_args.c \
		parse_options.c \
		main.c
