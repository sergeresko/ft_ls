/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   foreach.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/30 12:49:06 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/30 13:38:52 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	foreach(struct s_list *head, void (*func)(struct s_list *, void *), void *param)
{
	struct s_list	*elem;
	struct s_list	*next;

	elem = head->next;
	while (elem != head)
	{
		next = elem->next;
		func(elem, param);
		elem = next;
	}
}

void	foreach_bkwd(struct s_list *head, void (*func)(struct s_list *, void *), void *param)
{
	struct s_list	*elem;
	struct s_list	*prev;

	elem = head->prev;
	while (elem != head)
	{
		prev = elem->prev;
		func(elem, param);
		elem = prev;
	}
}
