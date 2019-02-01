/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list_short.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:16:03 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 14:26:34 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_ls.h"

static void	print_list_short_inner(struct s_list *elem, void *param)
{
	(void)param;

	if (g_metrics.s_len)
	{
		print_elem_info_short(elem);
		(void)write(1, g_metrics.s, g_metrics.s_len);
	}
	print_elem_name(elem);
}

void		print_list_short(struct s_list *head)
{
	compute_metrics(head);
	g_foreach_directed(head, print_list_short_inner, NULL);
}
