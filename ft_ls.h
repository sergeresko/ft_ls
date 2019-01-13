#ifndef FT_LS_H
# define FT_LS_H

# include <sys/stat.h>	// struct stat
# include <limits.h>	// PATH_MAX, NAME_MAX
# include <string.h>	// size_t for libft functions

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

/* list */

struct				s_list
{
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
	struct s_list	*prev;
	struct s_list	*next;
};

struct				s_metrics
{
	unsigned	inode_len;
	unsigned	nlink_len;
	unsigned	uname_len;
	unsigned	gname_len;
	unsigned	size_len;
	unsigned	name_len;
	unsigned	total_items;
	blkcnt_t	total_blocks;
};

extern char			g_path[PATH_MAX + NAME_MAX];
extern time_t		g_now;
extern unsigned		g_options;
extern time_t const	*(*g_time_func)(struct s_list *);

void				init(struct s_list *head);
//void			insert_after(struct s_list *item, struct s_list *new);
//void			delete(struct s_list *elem);

/* compare */
/*
int					compare_mtime(struct s_list *elem1, struct s_list *elem2);
int					compare_atime(struct s_list *elem1, struct s_list *elem2);
int					compare_ctime(struct s_list *elem1, struct s_list *elem2);
int					compare_birthtime(struct s_list *elem1, struct s_list *elem2);
int					compare_size(struct s_list *elem1, struct s_list *elem2);
*/

/* ------------------ libft functions ------------------ */

void				*ft_memcpy(void *dst, const void *src, size_t n);
size_t				ft_strlen(const char *s);
int					ft_strcmp(const char *s1, const char *s2);
unsigned			ft_utoa(char **addr, unsigned long long n);

/*
**	get_time.c
*/

time_t const	*get_mtime(struct s_list *elem);
time_t const	*get_atime(struct s_list *elem);
time_t const	*get_ctime(struct s_list *elem);
time_t const	*get_birthtime(struct s_list *elem);

/* options */

char const	**parse_options(char const **av);

// stat


// sort

void	sort_list(struct s_list *head);

// print_list

void	print_list_short(struct s_list *head);
void	print_list_short_reverse(struct s_list *head);
void		print_list_long(struct s_list *head, int show_total);
void		print_list_long_reverse(struct s_list *head, int show_total);


/* list_directory */

void		list_directory(int path_len);

/* --------- */

int		build_list_from_args(struct s_list *head, char const **av);
void	stat_list_arg(struct s_list *head);


void	list_args(char const **av);

#endif
