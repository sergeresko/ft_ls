/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list_short.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:16:03 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 17:19:00 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_ls.h"

void	print_list_short(struct s_list *head)
{
	struct s_metrics	metrics;
	unsigned			s_len;
	char				*s;
	struct s_list		*elem;
	struct s_list		*next;

	compute_metrics(&metrics, head);
	s_len = info_len(&metrics);
	if (s_len)
		s = (char *)malloc(s_len);
	elem = head->next;
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
	}
	if (s_len)
		free(s);
}

void	print_list_short_reverse(struct s_list *head)
{
	struct s_metrics	metrics;
	unsigned			s_len;
	char				*s;
	struct s_list		*elem;
	struct s_list		*prev;

	compute_metrics(&metrics, head);
	s_len = info_len(&metrics);
	if (s_len)
		s = (char *)malloc(s_len);
	elem = head->prev;
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
	}
	if (s_len)
		free(s);
}
