/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   foreach.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/30 12:49:06 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 17:16:26 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	foreach(t_list *head, void (*func)(t_list *, void *), void *param)
{
	t_list	*elem;
	t_list	*next;

	elem = head->next;
	while (elem != head)
	{
		next = elem->next;
		func(elem, param);
		elem = next;
	}
}

void	foreach_bkwd(t_list *head, void (*func)(t_list *, void *), void *param)
{
	t_list	*elem;
	t_list	*prev;

	elem = head->prev;
	while (elem != head)
	{
		prev = elem->prev;
		func(elem, param);
		elem = prev;
	}
}
