/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_arg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 15:28:10 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/31 15:13:53 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "ft_ls.h"

static void	init(struct s_list *head)
{
	head->prev = head;
	head->next = head;
}

static void	split_callback(struct s_list *elem, void *param)
{
	struct s_list *const	head_dir = (struct s_list *)param;

	if ((elem->stat.st_mode & S_IFMT) == S_IFDIR)
	{
		elem->prev->next = elem->next;
		elem->next->prev = elem->prev;
		elem->prev = head_dir->prev;
		elem->next = head_dir;
		head_dir->prev->next = elem;
		head_dir->prev = elem;
	}
}

static void	recursion_arg_callback(struct s_list *elem, void *param)
{
	static int	is_first = 1;
	int const	fmt = *(int *)param;

	(void)ft_memcpy(g_path, elem->name, elem->name_len + 1);
	if (fmt)
	{
		if (!is_first || fmt == 2)
			(void)write(1, "\n", 1);
		(void)write(1, g_path, ft_strlen(g_path));
		(void)write(1, ":\n", 2);
	}
	is_first = 0;
	list_directory(elem->name_len);
	free(elem->name);	// don't update
	free(elem);			// pointers
}

void		list_arg(char const **av)
{
	struct s_list	head;
	struct s_list	head_dir;
	int				fmt;

	init(&head);
	fmt = (build_list_arg(&head, av) > 1);
	foreach(&head, stat_arg_callback, NULL);
	if ((OPT & O_SORT) && (OPT & (O_SORT_TIME | O_SORT_SIZE)))
		foreach(&head, sort_callback, &head);
	init(&head_dir);
	foreach(&head, split_callback, &head_dir);
	fmt && (head.next != &head) && (++fmt);		// write in two lines
	if (OPT & O_LONG_FORMAT)
		print_list_long(&head, 0);
	else
		print_list_short(&head); 	// if (OPT & O_COLUMNS) TODO
	g_foreach_directed(&head_dir, recursion_arg_callback, &fmt);
}
