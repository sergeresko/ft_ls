/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:24:27 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/31 11:56:38 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	sort_list_inner(struct s_list *elem, void *param)
{
	struct s_list *const	head = (struct s_list *)param;	// not needed; inline in the following line (maybe even casting is not needed)
	struct s_list *const	after = g_after_func(head, elem);

//	after = elem->prev;
//	while (after != head && opt->compare_func(after, elem))
//		after = after->prev;
	if (after != elem->prev)
	{
		elem->next->prev = elem->prev;
		elem->prev->next = elem->next;
		elem->prev = after;
		elem->next = after->next;
		after->next->prev = elem;
		after->next = elem;
	}
}

/*
**	insertion sort
*/

void	sort_list(struct s_list *head)
{
	foreach(head, sort_list_inner, head);
}
