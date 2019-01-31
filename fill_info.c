/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_info.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 18:14:26 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/31 20:27:40 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	The following functions are used by stat_elem and stat_elem_arg.
**	fill_* append a terminating '\0'.
*/

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include "ft_ls.h"

void	fill_user(struct s_list *elem)
{
	struct passwd	*pw;

	if (!(OPT & O_NUM_UID_GID) && (pw = getpwuid(elem->stat.st_uid)) != NULL)
	{
		elem->uname_len = ft_strlen(pw->pw_name);
		elem->uname = (char *)malloc(elem->uname_len + 1);
		(void)ft_memcpy(elem->uname, pw->pw_name, elem->uname_len + 1);
	}
	else
	{
		errno = 0;
		elem->uname_len = ft_utoa(&(elem->uname), elem->stat.st_uid);
	}
}

void	fill_group(struct s_list *elem)
{
	struct group	*gr;

	if (!(OPT & O_NUM_UID_GID) && (gr = getgrgid(elem->stat.st_gid)) != NULL)
	{
		elem->gname_len = ft_strlen(gr->gr_name);
		elem->gname = (char *)malloc(elem->gname_len + 1);
		(void)ft_memcpy(elem->gname, gr->gr_name, elem->gname_len + 1);
	}
	else
	{
		errno = 0;
		elem->gname_len = ft_utoa(&(elem->gname), elem->stat.st_gid);
	}
}

/*
**	uses value in g_path
*/

int		fill_link(struct s_list *elem)
{
	unsigned	buf_size;

	if (readlink(g_path, NULL, 0) == -1)
	{
		file_error(g_path);
		elem->link = NULL;
		return (-1);
	}
	buf_size = 64;
	elem->link = (char *)malloc(buf_size);
	elem->link_len = (unsigned)readlink(g_path, elem->link, buf_size);
	while (elem->link_len == buf_size)	// >= is impossible
	{
		free(elem->link);
		buf_size *= 2;
		elem->link = (char *)malloc(buf_size);
		elem->link_len = (unsigned)readlink(g_path, elem->link, buf_size);
	}
	elem->link[elem->link_len] = '\0';
	return (0);	
}

/*
**	uses value in g_path
**	does not handle '+'
*/
char	xattr_acl(void)
{
	if (listxattr(g_path, NULL, 0, XATTR_NOFOLLOW) > 0)	// ?
		return ('@');
	return (' ');
}
