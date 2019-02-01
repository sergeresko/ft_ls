/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 15:08:22 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 17:18:40 by syeresko         ###   ########.fr       */
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
time_t const	*(*g_time_func)(t_list const *);
t_list			*(*g_after_func)(t_list const *, t_list const *);
void			(*g_foreach_directed)(t_list *head, t_callback func, void *param);		// new
t_metrics		g_metrics;

/*
**	Set the global variable g_time_func
*/

static void		set_time_func(void)
{
	if (OPT & O_MTIME)
		g_time_func = get_mtime;
	else if (OPT & O_CTIME)
		g_time_func = get_ctime;
	else if (OPT & O_ATIME)
		g_time_func = get_atime;
	else if (OPT & O_BIRTHTIME)
		g_time_func = get_birthtime;
}

static void		set_after_func(void)
{
	if (OPT & O_SORT_SIZE)
		g_after_func = after_size;	// what is size for special files?
	else if (OPT & O_SORT_TIME)
		g_after_func = after_time;
}

/*
**	main
*/

int	main(int argc, char const *argv[])
{
	(void)argc;
	(void)time(&g_now);
	argv = parse_options(argv);
	set_time_func();
	set_after_func();
	if ((OPT & O_SORT) && (OPT & O_SORT_REVERSE))		//
		g_foreach_directed = foreach_bkwd;				//
	else												//
		g_foreach_directed = foreach;					//
	g_metrics.allocated_len = 64;								//
	g_metrics.s = (char *)malloc(g_metrics.allocated_len);		//
	(void)list_arg(argv);
	free(g_metrics.s);
	//
//	printf("g_metrics.allocated_len == %u\n", g_metrics.allocated_len);	//
	system("leaks -q ft_ls");	//
	//
	return (0);
}
