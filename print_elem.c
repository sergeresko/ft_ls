/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_elem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:51:19 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 18:17:39 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_ls.h"

void		print_elem_info_short(t_list *elem)
{
	char	*s;

	if (OPT & O_SHOW_INODE)
	{
		s = g_metrics.s;
		s = sprint_u(s, g_metrics.inode_len, elem->stat.st_ino);
		*(s++) = ' ';
		(void)write(1, g_metrics.s, g_metrics.s_len);
	}
}

/*
**	frees uname and gname
*/

void	print_elem_info_long(t_list *elem)
{
	char	*s;

	s = g_metrics.s;
	if (OPT & O_SHOW_INODE)
	{
		s = sprint_u(s, g_metrics.inode_len, elem->stat.st_ino);
		*(s++) = ' ';
	}
	s = sprint_mode(s, elem);
	s = sprint_u(s, g_metrics.nlink_len, elem->stat.st_nlink);
	*(s++) = ' ';
	if (OPT & O_SHOW_USER)
	{
		s = sprint_s(s, elem->uname, elem->uname_len, g_metrics.uname_len + 2);
		free(elem->uname);
	}
	if (OPT & O_SHOW_GROUP)
	{
		s = sprint_s(s, elem->gname, elem->gname_len, g_metrics.gname_len + 2);
		free(elem->gname);
	}
	s = sprint_size(s, elem, g_metrics.size_len);
	s = sprint_time(s, g_time_func(elem));
	*(s++) = ' ';
	(void)write(1, g_metrics.s, g_metrics.s_len);
}

/*
**	frees name, link and elem itself
*/

void	print_elem_name(t_list *elem)
{
	(void)write(1, elem->name, elem->name_len);
	if ((OPT & O_LONG_FORMAT) && elem->link)	// ... != NULL
	{
		(void)write(1, " -> ", 4);
		(void)write(1, elem->link, elem->link_len);
		free(elem->link);
	}
	if (!(OPT & O_RECURSIVE) ||
		(elem->stat.st_mode & S_IFMT) != S_IFDIR ||
		is_dummy(elem->name))
	{
		elem->prev->next = elem->next;
		elem->next->prev = elem->prev;
		free(elem->name);
		free(elem);
	}
}
