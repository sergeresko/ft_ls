/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metrics_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:46:06 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 17:20:10 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

unsigned	max_uname_len(t_list const *head)
{
	unsigned		max;
	t_list const	*elem;

	max = 0;
	elem = head->next;
	while (elem != head)
	{
		if (elem->uname_len > max)
			max = elem->uname_len;
		elem = elem->next;
	}
	return (max);
}

unsigned	max_gname_len(t_list const *head)
{
	unsigned		max;
	t_list const	*elem;

	max = 0;
	elem = head->next;
	while (elem != head)
	{
		if (elem->gname_len > max)
			max = elem->gname_len;
		elem = elem->next;
	}
	return (max);
}

unsigned	max_name_len(t_list const *head)
{
	unsigned		max;
	t_list const	*elem;

	max = 0;
	elem = head->next;
	while (elem != head)
	{
		if (elem->name_len > max)
			max = elem->name_len;
		elem = elem->next;
	}
	return (max);
}

unsigned	total_items(t_list const *head)
{
	unsigned		sum;
	t_list const	*elem;

	sum = 0;
	elem = head->next;
	while (elem != head)
	{
		++sum;
		elem = elem->next;
	}
	return (sum);

}

blkcnt_t	total_blocks(t_list const *head)
{
	blkcnt_t		sum;
	t_list const	*elem;

	sum = 0;
	elem = head->next;
	while (elem != head)
	{
		sum += elem->stat.st_blocks;
		elem = elem->next;
	}
	return (sum);
}
