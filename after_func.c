/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   after_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:14:01 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 17:11:59 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_list	*after_mtime(t_list const *head, t_list const *elem)
{
	time_t const	elem_mtime = elem->stat.st_mtime;
	t_list			*after;

	after = elem->prev;
	while (after != head && after->stat.st_mtime < elem_mtime)
		after = after->prev;
	return (after);
}

t_list	*after_ctime(t_list const *head, t_list const *elem)
{
	time_t const	elem_ctime = elem->stat.st_ctime;
	t_list			*after;

	after = elem->prev;
	while (after != head && after->stat.st_ctime < elem_ctime)
		after = after->prev;
	return (after);
}

t_list	*after_atime(t_list const *head, t_list const *elem)
{
	time_t const	elem_atime = elem->stat.st_atime;
	t_list			*after;

	after = elem->prev;
	while (after != head && after->stat.st_atime < elem_atime)
		after = after->prev;
	return (after);
}

t_list	*after_birthtime(t_list const *head, t_list const *elem)
{
	time_t const	elem_birthtime = elem->stat.st_birthtime;
	t_list			*after;

	after = elem->prev;
	while (after != head && after->stat.st_birthtime < elem_birthtime)
		after = after->prev;
	return (after);
}

t_list	*after_size(t_list const *head, t_list const *elem)
{
	off_t const		elem_size = elem->stat.st_size;
	t_list			*after;

	after = elem->prev;
	while (after != head && after->stat.st_size < elem_size)
		after = after->prev;
	return (after);
}
