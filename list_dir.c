/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:02:29 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/31 20:04:35 by syeresko         ###   ########.fr       */
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

static void	recursion_callback(struct s_list *elem, void *param)
{
	int const	path_len = *(int *)param;

	(void)ft_memcpy(g_path + path_len, elem->name, elem->name_len + 1);
	(void)write(1, "\n", 1);
	(void)write(1, g_path, path_len + elem->name_len);
	(void)write(1, ":\n", 2);
	if (list_directory(path_len + elem->name_len))
		file_error(elem->name);
	free(elem->name);	// don't update the pointers, since
	free(elem);			// this list won't be used further
}

int			list_directory(int path_len)
//void		list_directory(int path_len)
{
	struct s_list	head;

	init(&head);	// may inline
	if (build_list(&head))
		return (-1);
	g_path[path_len++] = '/';
	foreach(&head, stat_callback, &path_len);
	if ((OPT & O_SORT) && (OPT & (O_SORT_TIME | O_SORT_SIZE)))
		foreach(&head, sort_callback, &head);
	if (OPT & O_LONG_FORMAT)
		print_list_long(&head, 1);
	else	// if (OPT & O_COLUMNS) TODO
		print_list_short(&head);
	if (OPT & O_RECURSIVE)
		//g_path[path_len++] = '/';		// this has already been done above
		g_foreach_directed(&head, recursion_callback, &path_len);
	return (0);
}
