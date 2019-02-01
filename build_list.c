/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 18:39:04 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 17:14:29 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>		// opendir, readdir, closedir
//#include <sys/stat.h>	// stat, lstat
//#include <unistd.h>		// readlink
//#include <stdio.h>		// perror
//#include <errno.h>		// errno
#include <stdlib.h>		// malloc, free, exit
//#include <time.h>
//#include <pwd.h>		// getpwuid
//#include <grp.h>		// getgrgid
//#include <sys/xattr.h>	// listxattr

#include "ft_ls.h"

/* ------------------------------------------- */
/*
void	file_error(char const *name)
{
	char const	*s = strerror(errno);

	(void)write(2, "./ft_ls: ", 9);
	(void)write(2, name, ft_strlen(name));
	(void)write(2, ": ", 2);
	(void)write(2, s, ft_strlen(s));
	(void)write(2, "\n", 1);
	errno = 0;
}
*/

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
//	if (!dirp)
		return (-1);
	while ((entry = readdir(dirp)) != NULL)
//	while ((entry = readdir(dirp)))
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

/* ------------------- functions for stat -------------------- */
/*
// With a terminating '\0', but do I need it?
// used in stat_elem
void	fill_user(struct s_list *elem)
{
	struct passwd	*pw;

	if (!(OPT & O_NUM_UID_GID) && (pw = getpwuid(elem->stat.st_uid)))
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

// With a terminating '\0', but do I need it?
// used in stat_elem
void	fill_group(struct s_list *elem)
{
	struct group	*gr;

	if (!(OPT & O_NUM_UID_GID) && (gr = getgrgid(elem->stat.st_gid)))
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

// With a terminating '\0', but do I need it?
// used in stat_elem
// uses value in g_path
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

// used in stat_elem
// uses value in g_path
// do not handle '+'
char	xattr_acl(void)
{
	if (listxattr(g_path, NULL, 0, XATTR_NOFOLLOW) > 0)	// ?
		return ('@');
	return (' ');
}
*/

/*
// used in stat_list
// uses value in g_path
// Maybe group in one file:
// 		static fill_user,
// 		static fill_group,
// 		static fill_link,
// 		static xattr_acl,
// 		stat_elem.
void	stat_elem(struct s_list *elem)
{
	if (lstat(g_path, &(elem->stat)) == -1)			// always lstat
	{		// display error and delete element
		file_error(g_path);
		elem->next->prev = elem->prev;
		elem->prev->next = elem->next;
		free(elem->name);
		free(elem);
	}
	else if (OPT & O_LONG_FORMAT)
	{
		if (OPT & O_SHOW_USER)
			fill_user(elem);
		if (OPT & O_SHOW_GROUP)
			fill_group(elem);
		if ((elem->stat.st_mode & S_IFMT) == S_IFLNK)
			fill_link(elem);		// not checking if it failed
		else
			elem->link = NULL;		//
		elem->xattr_acl = xattr_acl();
	}
}

// this does not need a reverse version
// writes to g_path
void	stat_list(struct s_list *head, int path_len)
{
	struct s_list	*elem;
	struct s_list	*next;

	g_path[path_len++] = '/';
	elem = head->next;
	while (elem != head)
	{
		next = elem->next;
		(void)ft_memcpy(g_path + path_len, elem->name, elem->name_len + 1);
		stat_elem(elem);
		elem = next;
	}
}
*/
/* ------------------------------------- */
/*
// used in print_mode
char	file_type(mode_t fmt)	// fmt = (st_mode & S_IFMT)
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
	return ('?');		// my
}
*/
/* --------------------------------------------- */

int			build_list_arg(t_list *head, char const **av)
{
	int		ac;

	ac = 0;
	if (*av == NULL)
//	if (!(*av))
		add_to_list(head, ".", 1);
	while (*av != NULL)
//	while (*av)
	{
		add_to_list(head, *av, ft_strlen(*av));
		++ac;
		++av;
	}
	return (ac);
}
/*
void	stat_elem_arg(struct s_list *elem)
{
	if (lstat(elem->name, &(elem->stat)) == -1)		// not g_path
	{		// display error and delete element
		file_error(elem->name);		// NB: filename is not in g_path !
		elem->next->prev = elem->prev;
		elem->prev->next = elem->next;
		free(elem->name);
		free(elem);
	}
	else
	{
		if (!(OPT & O_LONG_FORMAT) && (elem->stat.st_mode & S_IFMT) == S_IFLNK)
			(stat(elem->name, &(elem->stat)) == -1) && (errno = 0);
		if (OPT & O_LONG_FORMAT && (elem->stat.st_mode & S_IFMT) != S_IFDIR)
		{
			if (OPT & O_SHOW_USER)
				fill_user(elem);
			if (OPT & O_SHOW_GROUP)
				fill_group(elem);
			if ((elem->stat.st_mode & S_IFMT) == S_IFLNK)
				fill_link(elem);		// not checking if it failed
			else
				elem->link = NULL;		//
			elem->xattr_acl = xattr_acl();
		}
	}
}

void	stat_list_arg(struct s_list *head)
{
	struct s_list	*elem;
	struct s_list	*next;

	elem = head->next;
	while (elem != head)
	{
		next = elem->next;
		stat_elem_arg(elem);
		elem = next;
	}
}
*/
