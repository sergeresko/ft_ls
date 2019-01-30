/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list_long.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:21:24 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/30 14:50:00 by syeresko         ###   ########.fr       */
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

struct	s_print_list_param
{
	struct s_metrics	metrics;
	unsigned			s_len;
	char				*s;
};

void		print_list_long_inner(struct s_list *elem, void *param)
{
	struct s_print_list_param	*p = (struct s_print_list_param *)param;

	print_elem_info_long(p->s, elem, &(p->metrics));
	(void)write(1, p->s, p->s_len);
	print_elem_name(elem);

}

void		print_list_long(struct s_list *head, int show_total)
{
//	struct s_metrics	metrics;
//	unsigned			s_len;
//	char				*s;
	struct s_print_list_param	param;
//	struct s_list		*elem;
//	struct s_list		*next;

	compute_metrics(&(param.metrics), head);
	if (show_total && head->next != head)
		display_total(param.metrics.total_blocks);
	param.s_len = info_len(&(param.metrics));
	param.s = (char *)malloc(param.s_len);
/*	compute_metrics(&metrics, head);
	if (show_total && head->next != head)
		display_total(metrics.total_blocks);
	s_len = info_len(&metrics);
	s = (char *)malloc(s_len);*/
	foreach(head, print_list_long_inner, &param);
/*	elem = head->next;
	while (elem != head)
	{
		next = elem->next;
		print_elem_info_long(s, elem, &metrics);
		(void)write(1, s, s_len);
		print_elem_name(elem);
		elem = next;
	}*/
	free(param.s);
//	free(s);
}

void		print_list_long_reverse(struct s_list *head, int show_total)
{
//	struct s_metrics	metrics;
//	unsigned			s_len;
//	char				*s;
	struct s_print_list_param	param;
//	struct s_list		*elem;
//	struct s_list		*prev;

	compute_metrics(&(param.metrics), head);
	if (show_total && head->next != head)
		display_total(param.metrics.total_blocks);
	param.s_len = info_len(&(param.metrics));
	param.s = (char *)malloc(param.s_len);
	foreach_bkwd(head, print_list_long_inner, &param);
/*	elem = head->prev;
	while (elem != head)
	{
		prev = elem->prev;
		print_elem_info_long(s, elem, &metrics);
		(void)write(1, s, s_len);
		print_elem_name(elem);
		elem = prev;
	}*/
	free(param.s);
}
