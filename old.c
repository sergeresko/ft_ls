#include <dirent.h>		// opendir, readdir, closedir
#include <sys/stat.h>	// stat, lstat
#include <unistd.h>		// readlink
#include <stdio.h>		// perror
#include <errno.h>		// errno
#include <stdlib.h>		// malloc, free, exit
#include <time.h>
#include <pwd.h>		// getpwuid
#include <grp.h>		// getgrgid
#include <sys/xattr.h>	// listxattr

#include "ft_ls.h"

int		is_dummy(char const *name)
{
	return (name[0] == '.' && (!name[1] || (name[1] == '.' && !name[2])));
}

/* ------------------------------------------- */

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

// With a terminating '\0', but do I need it?
void	add_to_list(struct s_list *head, char const *name, unsigned name_len)
{
	struct s_list	*new;
	struct s_list	*after;

	new = (struct s_list *)malloc(sizeof(struct s_list));
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

int		build_list(struct s_list *head)
{
	DIR *const		dirp = opendir(g_path);
	struct dirent	*entry;

	if (!dirp)
	{
		file_error(g_path);
		return (-1);
	}
	while ((entry = readdir(dirp)))
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

/* ----------------- metrics --------------- */

// loops forever for huge numbers that cause overflow
// used in max_inode_len, max_nlink_len, max_size_len
unsigned	ndigits(unsigned long long n)
{
	unsigned	len;
	unsigned	power;

	len = 0;
	power = 1;
	while (n >= power)
	{
		++len;
		power *= 10;
	}
	return (len ? len : 1);
}

unsigned	max_inode_len(struct s_list const *head)
{
	ino_t				max;
	struct s_list const	*elem;

	max = 0;
	elem = head;
	while ((elem = elem->next) != head)
	{
		if (elem->stat.st_ino > max)
			max = elem->stat.st_ino;
	}
	return (ndigits(max));
}

unsigned	max_nlink_len(struct s_list const *head)
{
	nlink_t				max;
	struct s_list const	*elem;

	max = 0;
	elem = head;
	while ((elem = elem->next) != head)
	{
		if (elem->stat.st_nlink > max)
			max = elem->stat.st_nlink;
	}
	return (ndigits(max));
}

unsigned	max_uname_len(struct s_list const *head)
{
	unsigned			max;
	struct s_list const	*elem;

	max = 0;
	elem = head;
	while ((elem = elem->next) != head)
	{
		if (elem->uname_len > max)
			max = elem->uname_len;
	}
	return (max);
}

unsigned	max_gname_len(struct s_list const *head)
{
	unsigned			max;
	struct s_list const	*elem;

	max = 0;
	elem = head;
	while ((elem = elem->next) != head)
	{
		if (elem->gname_len > max)
			max = elem->gname_len;
	}
	return (max);
}

// doesn't handle human-readable option
unsigned	max_size_len(struct s_list const *head)
{
	off_t				max_size;
	int					is_special;
	struct s_list const	*elem;
	unsigned			max;

	max_size = 0;
	is_special = 0;
	elem = head;
	while ((elem = elem->next) != head)
	{
		if ((elem->stat.st_mode & S_IFMT) == S_IFCHR ||
			(elem->stat.st_mode & S_IFMT) == S_IFBLK)
		{
			is_special = 1;
		}
		else if (elem->stat.st_size > max_size)
			max_size = elem->stat.st_size;
	}
	if (is_special)
		return ((max = ndigits(max_size)) > 8 ? max : 8);
	return (ndigits(max_size));
}

unsigned	max_name_len(struct s_list const *head)
{
	unsigned			max;
	struct s_list const	*elem;

	max = 0;
	elem = head;
	while ((elem = elem->next) != head)
	{
		if (elem->name_len > max)
			max = elem->name_len;
	}
	return (max);
}

unsigned	total_items(struct s_list const *head)
{
	unsigned			sum;
	struct s_list const	*elem;

	sum = 0;
	elem = head;
	while ((elem = elem->next) != head)
		++sum;
	return (sum);

}

blkcnt_t	total_blocks(struct s_list const *head)
{
	blkcnt_t			sum;
	struct s_list const	*elem;

	sum = 0;
	elem = head;
	while ((elem = elem->next) != head)
		sum += elem->stat.st_blocks;
	return (sum);
}

void	compute_metrics(struct s_metrics *metrics, struct s_list const *head)
{
	if (OPT & O_SHOW_INODE)
		metrics->inode_len = max_inode_len(head);
	if (OPT & O_LONG_FORMAT)
	{
		metrics->total_blocks = total_blocks(head);
		metrics->nlink_len = max_nlink_len(head);
		if (OPT & O_SHOW_USER)
			metrics->uname_len = max_uname_len(head);
		if (OPT & O_SHOW_GROUP)
			metrics->gname_len = max_gname_len(head);
		metrics->size_len = max_size_len(head);
	}
	else if (OPT & O_COLUMNS)
	{
		metrics->total_items = total_items(head);
		metrics->name_len = max_name_len(head);
	}
}

/* ------------------------------------- */

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

// with a following space
char	*print_mode(char *dst, struct s_list const *elem)
{
	mode_t const	mode = elem->stat.st_mode;
	char			*s;	// may use just dst or, alternatively, *(s++)

	s = dst;			//
	s[0] = file_type(mode & S_IFMT);
	s[1] = (mode & S_IRUSR) ? 'r' : '-';
	s[2] = (mode & S_IWUSR) ? 'w' : '-';
	if (mode & S_ISUID)
		s[3] = (mode & S_IXUSR) ? 's' : 'S';
	else
		s[3] = (mode & S_IXUSR) ? 'x' : '-';
	s[4] = (mode & S_IRGRP) ? 'r' : '-';
	s[5] = (mode & S_IWGRP) ? 'w' : '-';
	if (mode & S_ISGID)
		s[6] = (mode & S_IXGRP) ? 's' : 'S';
	else
		s[6] = (mode & S_IXGRP) ? 'x' : '-';
	s[7] = (mode & S_IROTH) ? 'r' : '-';
	s[8] = (mode & S_IWOTH) ? 'w' : '-';
	if (mode & S_ISVTX)
		s[9] = (mode & S_IXOTH) ? 't' : 'T';
	else
		s[9] = (mode & S_IXOTH) ? 'x' : '-';
	s[10] = elem->xattr_acl;
	s[11] = ' ';
	return (dst + 12);
}

// with a space before
char	*print_time(char *dst, time_t const *time)
{
	char *const s = ctime(time);

	if (OPT & O_LONG_TIME)
	{
		(void)ft_memcpy(dst, s + 3, 21);
		return (dst + 21);
	}
	if (*time > g_now - SIX_MONTHS && *time < g_now + SIX_MONTHS)
		(void)ft_memcpy(dst, s + 3, 13);
	else
	{
		(void)ft_memcpy(dst, s + 3, 8);
		(void)ft_memcpy(dst + 8, s + 19, 5);
	}
	return (dst + 13);
}

// right align
char	*print_u(char *dst, unsigned width, unsigned n)
{
	char	*s;

	s = dst + width;
	if (n == 0)
		*(--s) = '0';
	while (n)
	{
		*(--s) = '0' + (char)(n % 10);
		n /= 10;
	}
	while (s != dst)
		*(--s) = ' ';
	return (dst + width);
}

// left align
char	*print_s(char *dst, char const *str, unsigned str_len, unsigned width)
{
	char	*s;

	(void)ft_memcpy(dst, str, str_len);
	s = dst + str_len;
	while (str_len++ < width)
		*(s++) = ' ';
	return (dst + width);
}

char	*print_size(char *dst, struct s_list const *elem, unsigned width)
{
	char	*s;

	if ((elem->stat.st_mode & S_IFMT) == S_IFCHR ||
		(elem->stat.st_mode & S_IFMT) == S_IFBLK)
	{
		s = dst + width - 8;
		(void)print_u(s, 3, elem->stat.st_rdev & 0xff);
		s[3] = ',';
		s[4] = ' ';
		(void)print_u(s + 5, 3, (elem->stat.st_rdev >> 8) & 0x7f);
		s[8] = ' ';
	}
	else
		(void)print_u(dst, width, elem->stat.st_size); 
	return (dst + width);
}

// used once in print_list_long{_reverse}
// used once in print_list_short{_reverse}
unsigned	stat_len(struct s_metrics const *metrics)
{
	unsigned	len;

	len = 0;
	if (OPT & O_SHOW_INODE)
		len += metrics->inode_len + 1;
	if (OPT & O_LONG_FORMAT)
	{
		len += metrics->nlink_len;
		if (OPT & O_SHOW_USER)
			len += metrics->uname_len + 2;
		if (OPT & O_SHOW_GROUP)
			len += metrics->gname_len + 2;
		len += metrics->size_len;
		len += (OPT & O_LONG_TIME) ? 20 : 12;
		return (len + 12 + 1 + 2);
		// 12 (mode and space) + 1 (after nlink) + 2 (around time)
	}
	return (len);
}

/* ------------ file: print_elem.c ---------------- */

void	print_elem_info_short(char *s, struct s_list *elem,
												struct s_metrics const *metrics)
{
	if (OPT & O_SHOW_INODE)
	{
		s = print_u(s, metrics->inode_len, elem->stat.st_ino);
		*(s++) = ' ';
	}
}

// frees uname and gname
void	print_elem_info_long(char *s, struct s_list *elem,
												struct s_metrics const *metrics)
{
	if (OPT & O_SHOW_INODE)
	{
		s = print_u(s, metrics->inode_len, elem->stat.st_ino);
		*(s++) = ' ';
	}
	s = print_mode(s, elem);
	s = print_u(s, metrics->nlink_len, elem->stat.st_nlink);
	*(s++) = ' ';
	if (OPT & O_SHOW_USER)
	{
		s = print_s(s, elem->uname, elem->uname_len, metrics->uname_len + 2);
		free(elem->uname);
	}
	if (OPT & O_SHOW_GROUP)
	{
		s = print_s(s, elem->gname, elem->gname_len, metrics->gname_len + 2);
		free(elem->gname);
	}
	s = print_size(s, elem, metrics->size_len);
	s = print_time(s, g_time_func(elem));
	*(s++) = ' ';
}

// frees name, link and elem itself
void	print_elem_name(struct s_list *elem)
{
	(void)write(1, elem->name, elem->name_len);
	if ((OPT & O_LONG_FORMAT) && elem->link)
	{
		(void)write(1, " -> ", 4);
		(void)write(1, elem->link, elem->link_len);
		free(elem->link);
	}
	(void)write(1, "\n", 1);
	if (!(OPT & O_RECURSIVE) ||
		(elem->stat.st_mode & S_IFMT) != S_IFDIR ||
		is_dummy(elem->name))
	{
		elem->prev->next = elem->next;
		elem->next->prev = elem->prev;
		free(elem->name);
		free(elem);
	}
}

/* ------------- file: print_list_short.c -------------- */

// also reverse
void	print_list_short(struct s_list *head)
{
	struct s_metrics	metrics;
	unsigned			s_len;
	char				*s;
	struct s_list		*elem;
	struct s_list		*next;

	compute_metrics(&metrics, head);
	s_len = stat_len(&metrics);
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
	s_len = stat_len(&metrics);
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

/* ------- file: print_list_long.c -------- */

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

// prints "total ..." if needed
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
	s_len = stat_len(&metrics);
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
	s_len = stat_len(&metrics);
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

/* --------------------------------------------- */

int		build_list_from_args(struct s_list *head, char const **av)
{
	int		ac;

	ac = 0;
	if (!(*av))
		add_to_list(head, ".", 1);
	while (*av)
	{
		add_to_list(head, *av, ft_strlen(*av));		// (void)... ?
		++ac;
		++av;
	}
	return (ac);
}

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
