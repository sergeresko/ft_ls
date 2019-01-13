/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_metrics.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:47:09 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 17:47:41 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	compute_metrics(struct s_metrics *metrics, struct s_list const *head)
{
	if (OPT & O_SHOW_INODE)
		metrics->inode_len = max_inode_len(head);
	if (OPT & O_LONG_FORMAT)
	{
		metrics->total_blocks = total_blocks(head);
		metrics->nlink_len = max_nlink_len(head);
		if (OPT & O_SHOW_USER)
			metrics->uname_len = max_uname_len(head);
		if (OPT & O_SHOW_GROUP)
			metrics->gname_len = max_gname_len(head);
		metrics->size_len = max_size_len(head);
	}
	else if (OPT & O_COLUMNS)
	{
		metrics->total_items = total_items(head);
		metrics->name_len = max_name_len(head);
	}
}
