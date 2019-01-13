/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 15:18:53 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/13 15:20:01 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**
*/

time_t const	*get_mtime(struct s_list *elem)
{
	return (&(elem->stat.st_mtime));
}

time_t const	*get_atime(struct s_list *elem)
{
	return (&(elem->stat.st_atime));
}

time_t const	*get_ctime(struct s_list *elem)
{
	return (&(elem->stat.st_ctime));
}

time_t const	*get_birthtime(struct s_list *elem)
{
	return (&(elem->stat.st_birthtime));
}
