/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 15:18:53 by syeresko          #+#    #+#             */
/*   Updated: 2019/02/01 17:16:43 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

time_t const	*get_mtime(t_list *elem)
{
	return (&(elem->stat.st_mtime));
}

time_t const	*get_atime(t_list *elem)
{
	return (&(elem->stat.st_atime));
}

time_t const	*get_ctime(t_list *elem)
{
	return (&(elem->stat.st_ctime));
}

time_t const	*get_birthtime(t_list *elem)
{
	return (&(elem->stat.st_birthtime));
}
