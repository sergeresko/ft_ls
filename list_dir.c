/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:02:29 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/30 19:52:38 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_ls.h"

static void	init(struct s_list *head)
{
	head->prev = head;
	head->next = head;
}

void		recursion_inner(struct s_list *elem, void *param)
{
	int const	path_len = *(int *)param;

	(void)ft_memcpy(g_path + path_len, elem->name, elem->name_len + 1);
	(void)write(1, "\n", 1);
	(void)write(1, g_path, path_len + elem->name_len);
	(void)write(1, ":\n", 2);
	list_directory(path_len + elem->name_len);
	free(elem->name);	// don't update the pointers, since
	free(elem);			// this list won't be used further
}

// TODO: combine in one function
static void	recursion(struct s_list *head, int path_len)
{
	g_path[path_len++] = '/';
//	foreach(head, recursion_inner, &path_len);
	g_foreach_directed(head, recursion_inner, &path_len);
}

/*static void	recursion_reverse(struct s_list *head, int path_len)
{
	g_path[path_len++] = '/';
	foreach_bkwd(head, recursion_inner, &path_len);
}*/

void		list_directory(int path_len)
{
	struct s_list	head;

	init(&head);	// may inline
	build_list(&head);
	stat_list(&head, path_len);
	if ((OPT & O_SORT) && (OPT & (O_SORT_TIME | O_SORT_SIZE)))
		sort_list(&head);
//	if ((OPT & O_SORT) && (OPT & O_SORT_REVERSE))
//	{
//		if (OPT & O_LONG_FORMAT)
//			print_list_long_reverse(&head, 1);
//		else	// if (OPT & O_COLUMNS)	TODO
//			print_list_short_reverse(&head);
//		if (OPT & O_RECURSIVE)
//			recursion_reverse(&head, path_len);
//	}
//	else
//	{
		if (OPT & O_LONG_FORMAT)
			print_list_long(&head, 1);
		else	// if (OPT & O_COLUMNS) TODO
			print_list_short(&head);
		if (OPT & O_RECURSIVE)
			recursion(&head, path_len);
//	}
}
