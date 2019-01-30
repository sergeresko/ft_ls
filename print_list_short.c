/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list_short.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:16:03 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/30 19:47:21 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_ls.h"

void	print_list_short_inner(struct s_list *elem, void *param)
{
	struct s_metrics * const	metrics = (struct s_metrics *)param;

	if (metrics->s_len)
	{
		print_elem_info_short(metrics->s, elem, metrics);
		(void)write(1, metrics->s, metrics->s_len);
	}
	print_elem_name(elem);
}

// TODO: combine in one function
void	print_list_short(struct s_list *head)
{
	struct s_metrics	metrics;

	compute_metrics(&metrics, head);
	metrics.s_len = info_len(&metrics);
	if (metrics.s_len)
		metrics.s = (char *)malloc(metrics.s_len);
	g_foreach_directed(head, print_list_short_inner, &metrics);		// only this line is different
//	foreach(head, print_list_short_inner, &metrics);		// only this line is different
	if (metrics.s_len)
		free(metrics.s);
}

/*void	print_list_short_reverse(struct s_list *head)
{
	struct s_metrics	metrics;

	compute_metrics(&metrics, head);
	metrics.s_len = info_len(&metrics);
	if (metrics.s_len)
		metrics.s = (char *)malloc(metrics.s_len);
	foreach_bkwd(head, print_list_short_inner, &metrics);	// only this line is different
	if (metrics.s_len)
		free(metrics.s);
}*/
