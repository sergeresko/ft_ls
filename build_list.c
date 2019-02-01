/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 18:39:04 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 22:26:13 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>		// opendir, readdir, closedir
#include <stdlib.h>		// malloc, free, exit
#include "ft_ls.h"

/*
**	name is added with a terminating '\0'
*/

static void	add_to_list(t_list *head, char const *name, unsigned name_len)
{
	t_list	*new;
	t_list	*after;

	new = (t_list *)malloc(sizeof(t_list));
	new->name_len = name_len;
	new->name = (char *)malloc(name_len + 1);
	(void)ft_memcpy(new->name, name, name_len + 1);
	after = head->prev;
	if (OPT & O_SORT)
		while (after != head && ft_strcmp(after->name, name) > 0)
			after = after->prev;
	new->prev = after;
	new->next = after->next;
	after->next->prev = new;
	after->next = new;
}

int			build_list(t_list *head)
{
	DIR *const		dirp = opendir(g_path);
	struct dirent	*entry;

	if (dirp == NULL)
		return (-1);
	while ((entry = readdir(dirp)) != NULL)
	{
		if (entry->d_name[0] != '.' || (OPT & O_LIST_ALL) ||
			((OPT & O_LIST_HIDDEN) && !is_dummy(entry->d_name)))
		{
			add_to_list(head, entry->d_name, entry->d_namlen);
		}
	}
	(void)closedir(dirp);
	return (0);
}

int			build_list_arg(t_list *head, char const **av)
{
	int		ac;

	ac = 0;
	if (*av == NULL)
		add_to_list(head, ".", 1);
	while (*av != NULL)
	{
		add_to_list(head, *av, ft_strlen(*av));
		++ac;
		++av;
	}
	return (ac);
}
