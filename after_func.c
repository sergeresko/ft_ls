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

t_list	*after_time(t_list const *head, t_list const *elem)
{
	time_t const	elem_time = *g_time_func(elem);
	t_list			*after;

	after = elem->prev;
	while (after != head && *g_time_func(after) < elem_time)
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
