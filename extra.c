/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 18:26:50 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 22:05:16 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "ft_ls.h"

int		is_dummy(char const *name)
{
	return (name[0] == '.' && (!name[1] || (name[1] == '.' && !name[2])));
}

void	file_error(t_list const *elem)
{
	char const	*s = strerror(errno);

	(void)write(2, "./ft_ls: ", 9);
	(void)write(2, elem->name, elem->name_len);
	(void)write(2, ": ", 2);
	(void)write(2, s, ft_strlen(s));
	(void)write(2, "\n", 1);
	errno = 0;
}

/*
**	used in print_mode
**	fmt = (st_mode & S_IFMT)
*/

char	file_type(mode_t fmt)
{
	if (fmt == S_IFREG)
		return ('-');
	if (fmt == S_IFDIR)
		return ('d');
	if (fmt == S_IFLNK)
		return ('l');
	if (fmt == S_IFCHR)
		return ('c');
	if (fmt == S_IFBLK)
		return ('b');
	if (fmt == S_IFIFO)
		return ('p');
	if (fmt == S_IFSOCK)
		return ('s');
	return ('?');
}
