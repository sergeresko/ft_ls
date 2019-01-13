/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metrics_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:46:06 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 17:46:54 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

unsigned	max_uname_len(struct s_list const *head)
{
	unsigned			max;
	struct s_list const	*elem;

	max = 0;
	elem = head;
	while ((elem = elem->next) != head)
	{
		if (elem->uname_len > max)
			max = elem->uname_len;
	}
	return (max);
}

unsigned	max_gname_len(struct s_list const *head)
{
	unsigned			max;
	struct s_list const	*elem;

	max = 0;
	elem = head;
	while ((elem = elem->next) != head)
	{
		if (elem->gname_len > max)
			max = elem->gname_len;
	}
	return (max);
}

unsigned	max_name_len(struct s_list const *head)
{
	unsigned			max;
	struct s_list const	*elem;

	max = 0;
	elem = head;
	while ((elem = elem->next) != head)
	{
		if (elem->name_len > max)
			max = elem->name_len;
	}
	return (max);
}

unsigned	total_items(struct s_list const *head)
{
	unsigned			sum;
	struct s_list const	*elem;

	sum = 0;
	elem = head;
	while ((elem = elem->next) != head)
		++sum;
	return (sum);

}

blkcnt_t	total_blocks(struct s_list const *head)
{
	blkcnt_t			sum;
	struct s_list const	*elem;

	sum = 0;
	elem = head;
	while ((elem = elem->next) != head)
		sum += elem->stat.st_blocks;
	return (sum);
}
