/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:24:27 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 16:25:46 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static t_after_func	select_after_func(void)
{
	if (OPT & O_SORT_SIZE)
		return (after_size);	// what is size for special files?
	if (OPT & O_SORT_TIME)
	{
		if (OPT & O_MTIME)
			return (after_mtime);
		if (OPT & O_CTIME)
			return (after_ctime);
		if (OPT & O_ATIME)
			return (after_atime);
		if (OPT & O_BIRTHTIME)
			return (after_birthtime);
	}
	return (NULL);	// this shouldn't happen
}

/*
**	insertion sort
*/

void			sort_list(struct s_list *head)
{
	struct s_list		*elem;
	struct s_list		*next;
	struct s_list		*after;
	t_after_func const	after_func = select_after_func();

	elem = head->next;
	while (elem != head)
	{
		next = elem->next;
		after = after_func(head, elem);
//		after = elem->prev;
//		while (after != head && opt->compare_func(after, elem))
//			after = after->prev;
		if (after != elem->prev)
		{
			elem->next->prev = elem->prev;
			elem->prev->next = elem->next;
			elem->prev = after;
			elem->next = after->next;
			after->next->prev = elem;
			after->next = elem;
		}
		elem = next;
	}
}
