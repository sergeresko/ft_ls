/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metrics_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:43:38 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 15:12:32 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**	Loops forever for huge numbers that cause overflow.
**	Used in max_inode_len, max_nlink_len, max_size_len.
*/

static unsigned	ndigits(unsigned long long n)
{
	unsigned	len;
	unsigned	power;

	len = 0;
	power = 1;
	while (n >= power)
	{
		++len;
		power *= 10;
	}
	return (len ? len : 1);
}

unsigned		max_inode_len(struct s_list const *head)
{
	ino_t				max;
	struct s_list const	*elem;

	max = 0;
	elem = head->next;
	while (elem != head)
	{
		if (elem->stat.st_ino > max)
			max = elem->stat.st_ino;
		elem = elem->next;
	}
	return (ndigits(max));
}

unsigned		max_nlink_len(struct s_list const *head)
{
	nlink_t				max;
	struct s_list const	*elem;

	max = 0;
	elem = head->next;
	while (elem != head)
	{
		if (elem->stat.st_nlink > max)
			max = elem->stat.st_nlink;
		elem = elem->next;
	}
	return (ndigits(max));
}

/*
**	Doesn't handle human-readable option.
*/

unsigned		max_size_len(struct s_list const *head)
{
	off_t				max_size;
	int					is_special;
	struct s_list const	*elem;
	unsigned			max;

	max_size = 0;
	is_special = 0;
	elem = head->next;
	while (elem != head)
	{
		if ((elem->stat.st_mode & S_IFMT) == S_IFCHR ||
			(elem->stat.st_mode & S_IFMT) == S_IFBLK)
		{
			is_special = 1;
		}
		else if (elem->stat.st_size > max_size)
			max_size = elem->stat.st_size;
		elem = elem->next;
	}
	if (is_special)
		return ((max = ndigits(max_size)) > 8 ? max : 8);
	return (ndigits(max_size));
}
