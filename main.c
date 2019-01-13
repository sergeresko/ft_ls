/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 15:08:22 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 18:45:19 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
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
*/

//#include <stdlib.h>		// leaks
#include <time.h>
#include "ft_ls.h"

/*
**	Definitions of global variables
*/

// maybe make [2 * PATH_MAX + 1] ?
char			g_path[PATH_MAX + NAME_MAX];
time_t			g_now;
unsigned		g_options;
time_t const	*(*g_time_func)(struct s_list *);

/*
**	Set the global variable g_time_func
*/

static int		set_time_func(void)
{
	if (OPT & O_MTIME)
		g_time_func = get_mtime;
	else if (OPT & O_CTIME)
		g_time_func = get_ctime;
	else if (OPT & O_ATIME)
		g_time_func = get_atime;
	else if (OPT & O_BIRTHTIME)
		g_time_func = get_birthtime;
	else
		return (-1);
	return (0);
}

/*
**	main
*/

int	main(int argc, char const *argv[])
{
	(void)argc;
	(void)time(&g_now);
	argv = parse_options(argv);
	(void)set_time_func();
	list_arg(argv);
	//
//	system("leaks -q ft_ls");	//
	//
	return (0);
}
