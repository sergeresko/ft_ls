/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list_long.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:21:24 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 17:25:08 by syeresko         ###   ########.fr       */
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

void		print_list_long(struct s_list *head, int show_total)
{
	struct s_metrics	metrics;
	unsigned			s_len;
	char				*s;
	struct s_list		*elem;
	struct s_list		*next;

	compute_metrics(&metrics, head);
	if (show_total && head->next != head)
		display_total(metrics.total_blocks);
	s_len = info_len(&metrics);
	s = (char *)malloc(s_len);
	elem = head->next;
	while (elem != head)
	{
		next = elem->next;
		print_elem_info_long(s, elem, &metrics);
		(void)write(1, s, s_len);
		print_elem_name(elem);
		elem = next;
	}
	free(s);
}

void		print_list_long_reverse(struct s_list *head, int show_total)
{
	struct s_metrics	metrics;
	unsigned			s_len;
	char				*s;
	struct s_list		*elem;
	struct s_list		*prev;

	compute_metrics(&metrics, head);
	if (show_total && head->next != head)
		display_total(metrics.total_blocks);
	s_len = info_len(&metrics);
	s = (char *)malloc(s_len);
	elem = head->prev;
	while (elem != head)
	{
		prev = elem->prev;
		print_elem_info_long(s, elem, &metrics);
		(void)write(1, s, s_len);
		print_elem_name(elem);
		elem = prev;
	}
	free(s);
}
