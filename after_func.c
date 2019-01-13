/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   after_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:14:01 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 16:15:38 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

struct s_list	*after_mtime(struct s_list const *head,
													struct s_list const *elem)
{
	time_t const	elem_mtime = elem->stat.st_mtime;
	struct s_list	*after;

	after = elem->prev;
	while (after != head && after->stat.st_mtime < elem_mtime)
		after = after->prev;
	return (after);
}

struct s_list	*after_ctime(struct s_list const *head,
													struct s_list const *elem)
{
	time_t const	elem_ctime = elem->stat.st_ctime;
	struct s_list	*after;

	after = elem->prev;
	while (after != head && after->stat.st_ctime < elem_ctime)
		after = after->prev;
	return (after);
}

struct s_list	*after_atime(struct s_list const *head,
													struct s_list const *elem)
{
	time_t const	elem_atime = elem->stat.st_atime;
	struct s_list	*after;

	after = elem->prev;
	while (after != head && after->stat.st_atime < elem_atime)
		after = after->prev;
	return (after);
}

struct s_list	*after_birthtime(struct s_list const *head,
													struct s_list const *elem)
{
	time_t const	elem_birthtime = elem->stat.st_birthtime;
	struct s_list	*after;

	after = elem->prev;
	while (after != head && after->stat.st_birthtime < elem_birthtime)
		after = after->prev;
	return (after);
}

struct s_list	*after_size(struct s_list const *head,
													struct s_list const *elem)
{
	off_t const		elem_size = elem->stat.st_size;
	struct s_list	*after;

	after = elem->prev;
	while (after != head && after->stat.st_size < elem_size)
		after = after->prev;
	return (after);
}
