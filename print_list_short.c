/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list_short.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:16:03 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/30 14:58:10 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_ls.h"

struct	s_print_list_param
{
	struct s_metrics	metrics;
	unsigned			s_len;
	char				*s;
};

void	print_list_short_inner(struct s_list *elem, void *param)
{
	struct s_print_list_param	*p = (struct s_print_list_param *)param;

	if (p->s_len)
	{
		print_elem_info_short(p->s, elem, &(p->metrics));
		(void)write(1, p->s, p->s_len);
	}
	print_elem_name(elem);
}

void	print_list_short(struct s_list *head)
{
//	struct s_metrics	metrics;
//	unsigned			s_len;
//	char				*s;
	struct s_print_list_param	param;
//	struct s_list		*elem;
//	struct s_list		*next;

	compute_metrics(&(param.metrics), head);
	param.s_len = info_len(&(param.metrics));
	if (param.s_len)
		param.s = (char *)malloc(param.s_len);
	foreach(head, print_list_short_inner, &param);
/*	elem = head->next;
	while (elem != head)
	{
		next = elem->next;
		if (s_len)
		{
			print_elem_info_short(s, elem, &metrics);
			(void)write(1, s, s_len);
		}
		print_elem_name(elem);
		elem = next;
	}*/
	if (param.s_len)
		free(param.s);
}

void	print_list_short_reverse(struct s_list *head)
{
//	struct s_metrics	metrics;
//	unsigned			s_len;
//	char				*s;
	struct s_print_list_param	param;
//	struct s_list		*elem;
//	struct s_list		*prev;

	compute_metrics(&(param.metrics), head);
	param.s_len = info_len(&(param.metrics));
	if (param.s_len)
		param.s = (char *)malloc(param.s_len);
	foreach_bkwd(head, print_list_short_inner, &param);
/*	elem = head->prev;
	while (elem != head)
	{
		prev = elem->prev;
		if (s_len)
		{
			print_elem_info_short(s, elem, &metrics);
			(void)write(1, s, s_len);
		}
		print_elem_name(elem);
		elem = prev;
	}*/
	if (param.s_len)
		free(param.s);
}
