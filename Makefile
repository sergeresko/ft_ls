all:
	clang -Wall -Wextra -Werror -o ft_ls \
		ft.c \
		list.c \
		list_directory.c \
		get_time.c \
		main.c
