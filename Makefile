# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: syeresko <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/13 15:49:09 by syeresko          #+#    #+#              #
#    Updated: 2019/01/13 17:22:42 by syeresko         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls
CC = clang
CFLAGS = -Wall -Wextra -Werror
SRC = \
	ft.c \
	old.c \
	get_time.c \
	list_args.c \
	list_dir.c \
	parse_options.c \
	after_func.c \
	sort_list.c \
	print_elem.c \
	print_list_short.c \
	print_list_long.c \
	extra.c \
	main.c
OBJ_DIR = ./obj/
OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c ft_ls.h
	$(CC) -o $@ -c $<

clean:
	rm -f $(OBJ)

fclean:
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
