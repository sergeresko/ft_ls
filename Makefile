all:
	clang -Wall -Wextra -Werror -o ft_ls \
		ft.c \
		list.c \
		list_directory.c \
		get_time.c \
		list_args.c \
		main.c
