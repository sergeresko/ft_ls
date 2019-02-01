/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:24:27 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 17:22:38 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

//	TODO: rename this file

void	sort_callback(t_list *elem, void *param)
{
	t_list *const	after = g_after_func(param, elem);

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
