/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_metrics.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:47:09 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 22:26:48 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>		// free
#include "ft_ls.h"

//	moved here from print_elem.c
/*
**	Magic numbers: 12 (mode and space) + 1 (after nlink) + 2 (around time).
*/

static unsigned	info_len(void)
{
	unsigned	len;

	len = 0;
	if (OPT & O_SHOW_INODE)
		len += g_metrics.inode_len + 1;
	if (OPT & O_LONG_FORMAT)
	{
		len += g_metrics.nlink_len;
		if (OPT & O_SHOW_USER)
			len += g_metrics.uname_len + 2;
		if (OPT & O_SHOW_GROUP)
			len += g_metrics.gname_len + 2;
		len += g_metrics.size_len;
		len += (OPT & O_LONG_TIME) ? 20 : 12;
		return (len + 12 + 1 + 2);
	}
	return (len);
}

void			compute_metrics(t_list const *head)
{
	if (OPT & O_SHOW_INODE)
		g_metrics.inode_len = max_inode_len(head);
	if (OPT & O_LONG_FORMAT)
	{
		g_metrics.total_blocks = total_blocks(head);
		g_metrics.nlink_len = max_nlink_len(head);
		if (OPT & O_SHOW_USER)
			g_metrics.uname_len = max_uname_len(head);
		if (OPT & O_SHOW_GROUP)
			g_metrics.gname_len = max_gname_len(head);
		g_metrics.size_len = max_size_len(head);
	}
	else if (OPT & O_COLUMNS)
	{
		g_metrics.total_items = total_items(head);
		g_metrics.name_len = max_name_len(head);
	}
	g_metrics.s_len = info_len();
	if (g_metrics.s_len > g_metrics.allocated_len)
	{
		free(g_metrics.s);
		g_metrics.s = (char *)malloc(g_metrics.s_len);
		g_metrics.allocated_len = g_metrics.s_len;
	}
}
