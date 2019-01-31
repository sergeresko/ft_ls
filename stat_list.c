/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 18:06:01 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/31 12:20:37 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "ft_ls.h"

/*
**	uses value in g_path
*/
static void	stat_elem(struct s_list *elem)
{
	if (lstat(g_path, &(elem->stat)) == -1)			// always lstat
	{		// display error and delete element
		file_error(g_path);
		elem->next->prev = elem->prev;
		elem->prev->next = elem->next;
		free(elem->name);
		free(elem);
	}
	else if (OPT & O_LONG_FORMAT)
	{
		if (OPT & O_SHOW_USER)
			fill_user(elem);
		if (OPT & O_SHOW_GROUP)
			fill_group(elem);
		if ((elem->stat.st_mode & S_IFMT) == S_IFLNK)
			fill_link(elem);		// not checking if it failed
		else
			elem->link = NULL;		//
		elem->xattr_acl = xattr_acl();
	}
}

static void	stat_list_inner(struct s_list *elem, void *param)
{
	int const	path_len = *(int *)param;	// better to pass (g_path + path_len) as param

	(void)ft_memcpy(g_path + path_len, elem->name, elem->name_len + 1);
	stat_elem(elem);
}

/*
**	writes to g_path
*/

void		stat_list(struct s_list *head, int path_len)
{
	g_path[path_len++] = '/';
	foreach(head, stat_list_inner, &path_len);
}

static void	stat_elem_arg(struct s_list *elem)
{
	if (lstat(elem->name, &(elem->stat)) == -1)		// not g_path
	{		// display error and delete element
		file_error(elem->name);		// NB: filename is not in g_path !
		elem->next->prev = elem->prev;
		elem->prev->next = elem->next;
		free(elem->name);
		free(elem);
	}
	else
	{
		if (!(OPT & O_LONG_FORMAT) && (elem->stat.st_mode & S_IFMT) == S_IFLNK)
			(stat(elem->name, &(elem->stat)) == -1) && (errno = 0);
		if (OPT & O_LONG_FORMAT && (elem->stat.st_mode & S_IFMT) != S_IFDIR)
		{
			if (OPT & O_SHOW_USER)
				fill_user(elem);
			if (OPT & O_SHOW_GROUP)
				fill_group(elem);
			if ((elem->stat.st_mode & S_IFMT) == S_IFLNK)
				fill_link(elem);		// not checking if it failed
			else
				elem->link = NULL;		//
			elem->xattr_acl = xattr_acl();
		}
	}
}

static void	stat_list_arg_inner(struct s_list *elem, void *param)
{
	(void)param;
	stat_elem_arg(elem);
}

void		stat_list_arg(struct s_list *head)
{
	foreach(head, stat_list_arg_inner, NULL);
}
