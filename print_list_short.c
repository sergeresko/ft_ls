/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list_short.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:16:03 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 17:22:27 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_ls.h"

static void	print_list_short_inner(t_list *elem, void *param)
{
	(void)param;

	if (g_metrics.s_len)
		print_elem_info_short(elem);
	print_elem_name(elem);
	(void)write(1, "\n", 1);
}

void		print_list_short(t_list *head)
{
	compute_metrics(head);
	g_foreach_directed(head, print_list_short_inner, NULL);
}
