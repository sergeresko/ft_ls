/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list_long.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:21:24 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 14:26:32 by syeresko         ###   ########.fr       */
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
	(void)param;

	print_elem_info_long(elem);
	(void)write(1, g_metrics.s, g_metrics.s_len);
	print_elem_name(elem);

}

void		print_list_long(struct s_list *head, int show_total)
{
	compute_metrics(head);
	if (show_total && head->next != head)
		display_total(g_metrics.total_blocks);
	g_foreach_directed(head, print_list_long_inner, NULL);
}
