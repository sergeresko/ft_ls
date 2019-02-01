/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 18:06:01 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 20:53:21 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "ft_ls.h"

//	TODO: rename this file

/*
**	writes to g_path
*/

void	stat_callback(t_list *elem, void *param)
{
	(void)ft_memcpy(param, elem->name, elem->name_len + 1);
	if (lstat(g_path, &(elem->stat)) == -1)			// always lstat
	{		// display error and delete element
		file_error(elem->name, elem->name_len);
		elem->next->prev = elem->prev;
		elem->prev->next = elem->next;
		free(elem->name);
		free(elem);
	}
	else if (OPT & O_LONG_FORMAT)
	{
		if (OPT & O_SHOW_USER)
			fill_uname(elem);
		if (OPT & O_SHOW_GROUP)
			fill_gname(elem);
		if ((elem->stat.st_mode & S_IFMT) == S_IFLNK)
			fill_link(elem);		// not checking if it failed
		else
			elem->link = NULL;		//
		elem->xattr_acl = xattr_acl();
	}
}

//
//
//

void	stat_arg_callback(t_list *elem, void *param)
{
	(void)ft_memcpy(param, elem->name, elem->name_len + 1);
	if (lstat(g_path, &(elem->stat)) == -1)
	{		// display error and delete element
		file_error(elem->name, elem->name_len);
		elem->next->prev = elem->prev;
		elem->prev->next = elem->next;
		free(elem->name);
		free(elem);
	}
	else
	{
		if (!(OPT & O_LONG_FORMAT) && (elem->stat.st_mode & S_IFMT) == S_IFLNK)
			(stat(elem->name, &(elem->stat)) == -1) && (errno = 0);		// (stat(elem->name, &(elem->stat)) == 0) || (errno = 0);
		if (OPT & O_LONG_FORMAT && (elem->stat.st_mode & S_IFMT) != S_IFDIR)
		{
			if (OPT & O_SHOW_USER)
				fill_uname(elem);
			if (OPT & O_SHOW_GROUP)
				fill_gname(elem);
			if ((elem->stat.st_mode & S_IFMT) == S_IFLNK)
				fill_link(elem);		// not checking if it failed
			else
				elem->link = NULL;		//
			elem->xattr_acl = xattr_acl();
		}
	}
}
