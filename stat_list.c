/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 18:06:01 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 21:59:20 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "ft_ls.h"

//	TODO: rename this file

static void	display_error_and_delete(t_list *elem)
{
	file_error(elem->name, elem->name_len);
	elem->next->prev = elem->prev;
	elem->prev->next = elem->next;
	free(elem->name);
	free(elem);
}

/*
**	writes to g_path
*/

void	stat_callback(t_list *elem, void *param)
{
	(void)ft_memcpy(param, elem->name, elem->name_len + 1);
	if (lstat(g_path, &(elem->stat)) == -1)		// ... != 0
	{
		display_error_and_delete(elem);
		return ;
	}
	if (param == g_path)
	{
		if (!(OPT & O_LONG_FORMAT) && (elem->stat.st_mode & S_IFMT) == S_IFLNK)
			(stat(elem->name, &(elem->stat)) == 0) || (errno = 0);
		if ((elem->stat.st_mode & S_IFMT) == S_IFDIR)
			return ;
	}
	if (OPT & O_LONG_FORMAT)
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
