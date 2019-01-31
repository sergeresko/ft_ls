/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list_long.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:21:24 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/31 16:02:48 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_ls.h"

static void	display_total(unsigned long long n)
{
	char		buf[20];
	char		*s;
	unsigned	len;

	s = buf + 20;
	len = 0;
	while (n || !len)
	{
		*(--s) = '0' + (char)(n % 10);
		n /= 10;
		++len;
	}
	(void)write(1, "total ", 6);
	(void)write(1, s, len);
	(void)write(1, "\n", 1);
}

static void	print_list_long_inner(struct s_list *elem, void *param)
{
	struct s_metrics *const		metrics = (struct s_metrics *)param;

	print_elem_info_long(metrics->s, elem, metrics);
	(void)write(1, metrics->s, metrics->s_len);
	print_elem_name(elem);

}

void		print_list_long(struct s_list *head, int show_total)
{
	struct s_metrics	metrics;

	compute_metrics(&metrics, head);
	if (show_total && head->next != head)
		display_total(metrics.total_blocks);
	metrics.s_len = info_len(&metrics);
	metrics.s = (char *)malloc(metrics.s_len);
	g_foreach_directed(head, print_list_long_inner, &metrics);
	free(metrics.s);
}
