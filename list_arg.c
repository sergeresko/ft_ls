/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_arg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 15:28:10 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 20:00:04 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "ft_ls.h"

static void	init(t_list *head)
{
	head->prev = head;
	head->next = head;
}

static void	split_callback(t_list *elem, void *param)
{
	t_list *const	head_dir = (t_list *)param;

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

static void	recursion_arg_callback(t_list *elem, void *param)
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
	if (list_directory(elem->name_len))		// ... != 0
		file_error(elem->name);
	free(elem->name);	// don't update the pointers, since
	free(elem);			// this list won't be used further
}

int			list_arg(char const **av)
{
	t_list	head;
	t_list	head_dir;
	int		fmt;

	init(&head);
	fmt = (build_list_arg(&head, av) > 1);
	foreach(&head, stat_arg_callback, NULL);
	if ((OPT & O_SORT) && (OPT & (O_SORT_TIME | O_SORT_SIZE)))
		foreach(&head, sort_callback, &head);
	init(&head_dir);
	foreach(&head, split_callback, &head_dir);
	if (fmt && head.next != &head)
		++fmt;
	if (OPT & O_LONG_FORMAT)
		print_list_long(&head, 0);
	else
		print_list_short(&head); 	// if (OPT & O_COLUMNS) TODO
	g_foreach_directed(&head_dir, recursion_arg_callback, &fmt);
	return (0);
}

/*
**	meaning of 'fmt':
**
**	(number of arguments is > 1)
**          /           \
**	    no /             \ yes
**	      /               \
**	  fmt = 0     (among the arguments
**	              there was at least 1
**	              valid non-directory)
**	                   /        \
**	               no /          \ yes
**	                 /            \
**               fmt = 1        fmt = 2
*/
