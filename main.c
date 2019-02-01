/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 15:08:22 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 13:20:15 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>		// leaks
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
struct s_list	*(*g_after_func)(struct s_list const *, struct s_list const *);
void			(*g_foreach_directed)(struct s_list *head, void (*func)(struct s_list *, void *), void *param);		// new
struct s_metrics	g_metrics;

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

static int		set_after_func(void)
{
	if (OPT & O_SORT_SIZE)
		g_after_func = after_size;	// what is size for special files?
	else if (OPT & O_SORT_TIME)
	{
		if (OPT & O_MTIME)
			g_after_func = after_mtime;
		else if (OPT & O_CTIME)
			g_after_func = after_ctime;
		else if (OPT & O_ATIME)
			g_after_func = after_atime;
		else if (OPT & O_BIRTHTIME)
			g_after_func = after_birthtime;
		else
			return (-1);
	}
	return (0);	// can be uninitialized
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
	(void)set_after_func();
	if ((OPT & O_SORT) && (OPT & O_SORT_REVERSE))		//
		g_foreach_directed = foreach_bkwd;				//
	else												//
		g_foreach_directed = foreach;					//
	g_metrics.allocated_len = 10;								//
	g_metrics.s = (char *)malloc(g_metrics.allocated_len);		//
	(void)list_arg(argv);
	free(g_metrics.s);
	//
	system("leaks -q ft_ls");	//
	//
	return (0);
}
