/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_field.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 17:34:05 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 17:37:56 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <time.h>
#include "ft_ls.h"

/*
**	with a following space
*/

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

/*
**	with a space before
*/

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

/*
**	right align
*/

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

/*
**	left align
*/

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