/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syeresko <syeresko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 13:08:40 by syeresko          #+#    #+#             */
/*   Updated: 2019/01/30 21:16:09 by syeresko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	n_words;
	long	*long_dst;
	long	*long_src;
	char	*char_dst;
	char	*char_src;

	n_words = n >> 3;
	n -= (n_words << 3);
	long_dst = (long *)dst;
	long_src = (long *)src;
	while (n_words--)
		*(long_dst++) = *(long_src++);
	char_dst = (char *)long_dst;
	char_src = (char *)long_src;
	while (n--)
		*(char_dst++) = *(char_src++);
	return (dst);
}

/*void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*cast_dst;
	unsigned char	*cast_src;

	cast_dst = (unsigned char *)dst;
	cast_src = (unsigned char *)src;
	while (n--)
		*(cast_dst++) = *(cast_src++);
	return (dst);
}*/

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (*(s++))
		++len;
	return (len);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		++s1;
		++s2;
	}
	return ((int)(unsigned char)*s1 - (int)(unsigned char)*s2);
}

/*
**	The following function is used only in fill_user and fill_group.
*/
unsigned	ft_utoa(char **addr, unsigned long long n)
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
	*addr = (char *)malloc(len + 1);
	(void)ft_memcpy(*addr, s, len);
	(*addr)[len] = '\0';
	return (len);
}
