/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_options.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 15:45:32 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/30 20:30:19 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_ls.h"

static int set_option_1(char c)
{
	if (c == 'i')
		OPT |= O_SHOW_INODE;
	else if (c == 'l')
		OPT |= O_LONG_FORMAT;		// OPT &= ~O_COLUMNS; (?)
	else if (c == 'g')
	{
		OPT |= O_LONG_FORMAT;
		OPT &= ~O_SHOW_USER;
	}
	else if (c == 'o')
	{
		OPT |= O_LONG_FORMAT;
		OPT &= ~O_SHOW_GROUP;
	}
	else if (c == 'n')
	{
		OPT |= O_LONG_FORMAT;
		OPT |= O_NUM_UID_GID;
	}
	else if (c == 'T')
		OPT |= O_LONG_TIME;
	else
		return (-1);
	return (0);
}

static int	set_option_2(char c)
{
	if (c == 'a')
		OPT |= O_LIST_ALL;
	else if (c == 'A')
		OPT |= O_LIST_HIDDEN;
	else if (c == 'R')
		OPT |= O_RECURSIVE;		// !
	else if (c == 'c')
	{
		OPT &= ~(O_MTIME | O_ATIME | O_BIRTHTIME);
		OPT |= O_CTIME;
	}
	else if (c == 'u')
	{
		OPT &= ~(O_MTIME | O_CTIME | O_BIRTHTIME);
		OPT |= O_ATIME;
	}
	else if (c == 'U')
	{
		OPT &= ~(O_MTIME | O_CTIME | O_ATIME);
		OPT |= O_BIRTHTIME;
	}
	else
		return (-1);
	return (0);
}

static int	set_option_3(char c)
{
	if (c == 'f')
	{
		OPT &= ~O_SORT;		// need unset other options?
		OPT |= O_LIST_ALL;
	}
	else if (c == 't')
	{
		if ((OPT & O_SORT) && !(OPT & O_SORT_SIZE))
			OPT |= O_SORT_TIME;
	}
	else if (c == 'S')
	{
		if (OPT & O_SORT)
		{
			OPT &= ~O_SORT_TIME;
			OPT |= O_SORT_SIZE;
		}
	}
	else if (c == 'r')
		OPT |= O_SORT_REVERSE;	// need 'if (OPT & O_SORT)' ?
	else
		return (-1);
	return (0);
}

static void	options_error(char const *s)
{
	(void)write(2, "./ft_ls: illegal option -- ",  27);
	(void)write(2, s, 1);
	(void)write(2, "\n", 1);
	(void)write(2, "usage: ./ft_ls [-ARSTUacfgilnortu] [file ...]\n", 30 + 16);
}

char const	**parse_options(char const **av)
{
	char const	*s;

	OPT = O_COLUMNS | O_SHOW_USER | O_SHOW_GROUP | O_MTIME | O_SORT;
	while ((s = *(++av)) && *s == '-' && *(s + 1))
	{
		while (*(++s))
		{
			if (set_option_1(*s) && set_option_2(*s) && set_option_3(*s))
			{
				options_error(s);
				exit(1);
			}
		}
	}
	return (av);
}
