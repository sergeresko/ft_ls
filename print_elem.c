/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_elem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:51:19 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 16:59:44 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_ls.h"

void		print_elem_info_short(char *s, struct s_list *elem,
											struct s_metrics const *metrics)
{
	if (OPT & O_SHOW_INODE)
	{
		s = print_u(s, metrics->inode_len, elem->stat.st_ino);
		*(s++) = ' ';
	}
}

/*
**	frees uname and gname
*/

void	print_elem_info_long(char *s, struct s_list *elem,
											struct s_metrics const *metrics)
{
	if (OPT & O_SHOW_INODE)
	{
		s = print_u(s, metrics->inode_len, elem->stat.st_ino);
		*(s++) = ' ';
	}
	s = print_mode(s, elem);
	s = print_u(s, metrics->nlink_len, elem->stat.st_nlink);
	*(s++) = ' ';
	if (OPT & O_SHOW_USER)
	{
		s = print_s(s, elem->uname, elem->uname_len, metrics->uname_len + 2);
		free(elem->uname);
	}
	if (OPT & O_SHOW_GROUP)
	{
		s = print_s(s, elem->gname, elem->gname_len, metrics->gname_len + 2);
		free(elem->gname);
	}
	s = print_size(s, elem, metrics->size_len);
	s = print_time(s, g_time_func(elem));
	*(s++) = ' ';
}

/*
**	frees name, link and elem itself
*/

void	print_elem_name(struct s_list *elem)
{
	(void)write(1, elem->name, elem->name_len);
	if ((OPT & O_LONG_FORMAT) && elem->link)
	{
		(void)write(1, " -> ", 4);
		(void)write(1, elem->link, elem->link_len);
		free(elem->link);
	}
	(void)write(1, "\n", 1);
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

/*
**	Used in print_list{_short,_long}[_reverse].
**	Magic numbers: 12 (mode and space) + 1 (after nlink) + 2 (around time).
*/

unsigned	info_len(struct s_metrics const *metrics)
{
	unsigned	len;

	len = 0;
	if (OPT & O_SHOW_INODE)
		len += metrics->inode_len + 1;
	if (OPT & O_LONG_FORMAT)
	{
		len += metrics->nlink_len;
		if (OPT & O_SHOW_USER)
			len += metrics->uname_len + 2;
		if (OPT & O_SHOW_GROUP)
			len += metrics->gname_len + 2;
		len += metrics->size_len;
		len += (OPT & O_LONG_TIME) ? 20 : 12;
		return (len + 12 + 1 + 2);
	}
	return (len);
}
