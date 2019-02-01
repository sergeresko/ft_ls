/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:23:23 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 14:30:18 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <sys/stat.h>	// struct stat
# include <limits.h>	// PATH_MAX, NAME_MAX
# include <string.h>	// size_t for libft functions (TODO: needed?)

# define OPT			g_options

# define O_LIST_HIDDEN	(1 << 16)
# define O_LIST_ALL		(1 << 17)
# define O_RECURSIVE	(1 << 4)
# define O_COLUMNS		(1 << 8)
# define O_LONG_FORMAT	(1 << 0)
# define O_SHOW_INODE	(1 << 3)
# define O_SHOW_USER	(1 << 1)
# define O_SHOW_GROUP	(1 << 2)
# define O_NUM_UID_GID	(1 << 5)
# define O_LONG_TIME	(1 << 6)
# define O_MTIME		(1 << 9)
# define O_CTIME		(1 << 10)
# define O_ATIME		(1 << 11)
# define O_BIRTHTIME	(1 << 12)
# define O_SORT			(1 << 7)
# define O_SORT_TIME	(1 << 13)
# define O_SORT_SIZE	(1 << 14)
# define O_SORT_REVERSE	(1 << 15)

# define SIX_MONTHS		15768000

struct					s_list
{
	struct s_list	*prev;
	struct s_list	*next;
	char			*name;
	char			*link;
	char			*uname;
	char			*gname;
	unsigned		name_len;
	unsigned		link_len;
	unsigned		uname_len;
	unsigned		gname_len;
	struct stat		stat;
	char			xattr_acl;
};

struct					s_metrics
{
	unsigned		inode_len;
	unsigned		nlink_len;
	unsigned		uname_len;
	unsigned		gname_len;
	unsigned		size_len;
	unsigned		name_len;
	unsigned		total_items;
	blkcnt_t		total_blocks;
	unsigned			s_len;		//	added
	char				*s;			//	added
	unsigned		allocated_len;		//	added recently
};

/*
**	Declaration of global variables:
*/

extern char				g_path[PATH_MAX + NAME_MAX];
extern time_t			g_now;
extern unsigned			g_options;
extern time_t const		*(*g_time_func)(struct s_list *);
extern struct s_list	*(*g_after_func)(struct s_list const *, struct s_list const *);
extern void			(*g_foreach_directed)(struct s_list *head, void (*func)(struct s_list *, void *), void *param);		// new
extern struct s_metrics	g_metrics;

/*
**	libft functions from ft.c:
*/

void					*ft_memcpy(void *dst, void const *src, size_t n);
size_t					ft_strlen(char const *s);
int						ft_strcmp(char const *s1, char const *s2);
unsigned				ft_utoa(char **addr, unsigned long long n);

/*
**	Miscellaneous extra functions from extra.c:
*/

int						is_dummy(char const *name);
void					file_error(char const *name);
char					file_type(mode_t fmt);

/*
**	Functions from get_time.c:
*/

time_t const			*get_mtime(struct s_list *elem);
time_t const			*get_atime(struct s_list *elem);
time_t const			*get_ctime(struct s_list *elem);
time_t const			*get_birthtime(struct s_list *elem);

/* options */

char const				**parse_options(char const **av);

/*
**	Functions for statistics from fill_info.c and stat_list.c:
*/
void					fill_user(struct s_list *elem);
void					fill_group(struct s_list *elem);
int						fill_link(struct s_list *elem);
char					xattr_acl(void);
//void					stat_list(struct s_list *head, int path_len);
//void					stat_list_arg(struct s_list *head);

/*
**	Functions for sorting from after_func.c and sort_list.c:
*/

struct s_list			*after_mtime(struct s_list const *head,
													struct s_list const *elem);
struct s_list			*after_ctime(struct s_list const *head,
													struct s_list const *elem);
struct s_list			*after_atime(struct s_list const *head,
													struct s_list const *elem);
struct s_list			*after_birthtime(struct s_list const *head,
													struct s_list const *elem);
struct s_list			*after_size(struct s_list const *head,
													struct s_list const *elem);
void					sort_list(struct s_list *head);

/*
**	Functions for metrics from metrics_{1,2}.c compute_metrics.c:
*/

unsigned				max_inode_len(struct s_list const *head);
unsigned				max_nlink_len(struct s_list const *head);
unsigned				max_uname_len(struct s_list const *head);
unsigned				max_gname_len(struct s_list const *head);
unsigned				max_size_len(struct s_list const *head);
unsigned				max_name_len(struct s_list const *head);
unsigned				total_items(struct s_list const *head);
blkcnt_t				total_blocks(struct s_list const *head);
void					compute_metrics(struct s_list const *head);

/*
**	Functions from print_field.c:
*/

char					*print_mode(char *dst, struct s_list const *elem);
char					*print_time(char *dst, time_t const *time);
char					*print_size(char *dst, struct s_list const *elem,
																unsigned width);
char					*print_s(char *dst, char const *str, unsigned str_len,
																unsigned width);
char					*print_u(char *dst, unsigned width, unsigned n);

/*
**	Functions from print_elem.c:
*/

void					print_elem_info_short(struct s_list *elem);
void					print_elem_info_long(struct s_list *elem);
void					print_elem_name(struct s_list *elem);

/*
**	Functions from print_list_{long,short}:
*/

void					print_list_short(struct s_list *head);
void					print_list_long(struct s_list *head, int show_total);

/*
**	Functions form build_list.c:
*/

int						build_list(struct s_list *head);
int						build_list_arg(struct s_list *head, char const **av);

/*
**	Functions from list_{arg,dir}.c:
*/

int						list_directory(int path_len);
int						list_arg(char const **av);

//
//	foreach
//
void	foreach(struct s_list *head, void (*func)(struct s_list *, void *), void *param);
void	foreach_bkwd(struct s_list *head, void (*func)(struct s_list *, void *), void *param);
void	sort_callback(struct s_list *elem, void *param);
void	stat_callback(struct s_list *elem, void *param);
void	stat_arg_callback(struct s_list *elem, void *param);

#endif
