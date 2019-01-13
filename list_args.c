/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 15:28:10 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 16:35:28 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include <dirent.h>		// opendir, readdir, closedir
#include <sys/stat.h>	// stat, lstat
#include <unistd.h>		// readlink
#include <stdio.h>		// perror
#include <errno.h>		// errno
#include <stdlib.h>		// malloc, free, exit
#include <time.h>
#include <pwd.h>		// getpwuid
#include <grp.h>		// getgrgid
#include <sys/xattr.h>	// listxattr
*/

#include <stdlib.h>
#include <unistd.h>
#include "ft_ls.h"

static void	init(struct s_list *head)
{
	head->prev = head;
	head->next = head;
}

static void	split_list(struct s_list *head, struct s_list *head_dir)
{
	struct s_list	*elem;
	struct s_list	*next;

	init(head_dir);
	elem = head->next;
	while (elem != head)
	{
		next = elem->next;
		if ((elem->stat.st_mode & S_IFMT) == S_IFDIR)
		{
			elem->prev->next = elem->next;
			elem->next->prev = elem->prev;
			elem->prev = head_dir->prev;
			elem->next = head_dir;
			head_dir->prev->next = elem;
			head_dir->prev = elem;
		}
		elem = next;
	}
}

static void	recursion_args(struct s_list *head, int fmt)
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

static void	recursion_args_reverse(struct s_list *head, int fmt)
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

void		list_args(char const **av)
{
	struct s_list	head;
	struct s_list	head_dir;
	int				fmt;

	init(&head);
	fmt = (build_list_from_args(&head, av) > 1);
	stat_list_arg(&head);
	if ((OPT & O_SORT) && (OPT & (O_SORT_TIME | O_SORT_SIZE)))
		sort_list(&head);
	split_list(&head, &head_dir);
	fmt && (head.next != &head) && (++fmt);
	if ((OPT & O_SORT) && (OPT & O_SORT_REVERSE))
	{
		(OPT & O_LONG_FORMAT) ?
			print_list_long_reverse(&head, 0) :
			print_list_short_reverse(&head); 	// if (OPT & O_COLUMNS) TODO
		recursion_args_reverse(&head_dir, fmt);
	}
	else
	{
		(OPT & O_LONG_FORMAT) ?
			print_list_long(&head, 0) :
			print_list_short(&head);		// if (OPT & O_COLUMNS) TODO
		recursion_args(&head_dir, fmt);
	}
}
