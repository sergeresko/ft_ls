/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_arg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 15:28:10 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/31 13:42:44 by syeresko         ###   ########.fr       */
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

// TODO:
/*void		recursion_arg_inner(struct s_list *elem, void *param)
{
	int const	fmt = *(int *)param;

	(void)ft_memcpy(g_path, elem->name, elem->name_len + 1);
	if (fmt)
	{
		if (elem != head->next || fmt == 2)
			(void)write(1, "\n", 1);
		(void)write(1, g_path, ft_strlen(g_path));
		(void)write(1, ":\n", 2);
	}
	list_directory(elem->name_len);
	free(elem->name);	// don't update
	free(elem);			// pointers
}*/

static void	recursion_arg(struct s_list *head, int fmt)
{
	struct s_list	*elem;
	struct s_list	*next;

	elem = head->next;
	while (elem != head)
	{
		next = elem->next;
		(void)ft_memcpy(g_path, elem->name, elem->name_len + 1);
		if (fmt)
		{
			if (elem != head->next || fmt == 2)
				(void)write(1, "\n", 1);
			(void)write(1, g_path, ft_strlen(g_path));
			(void)write(1, ":\n", 2);
		}
		list_directory(elem->name_len);
		free(elem->name);	// don't update
		free(elem);			// pointers
		elem = next;
	}
}

static void	recursion_arg_reverse(struct s_list *head, int fmt)
{
	struct s_list	*elem;
	struct s_list	*prev;

	elem = head->prev;
	while (elem != head)
	{
		prev = elem->prev;
		(void)ft_memcpy(g_path, elem->name, elem->name_len + 1);
		if (fmt)
		{
			if (elem != head->prev || fmt == 2)
				(void)write(1, "\n", 1);
			(void)write(1, g_path, ft_strlen(g_path));
			(void)write(1, ":\n", 2);
		}
		list_directory(elem->name_len);
		free(elem->name);	// don't update
		free(elem);			// pointers
		elem = prev;
	}
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
	fmt && (head.next != &head) && (++fmt);
	if (OPT & O_LONG_FORMAT)
		print_list_long(&head, 0);
	else
		print_list_short(&head); 	// if (OPT & O_COLUMNS) TODO
	// TODO:
	if ((OPT & O_SORT) && (OPT & O_SORT_REVERSE))
		recursion_arg_reverse(&head_dir, fmt);
	else
		recursion_arg(&head_dir, fmt);
}
