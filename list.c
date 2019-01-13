#include <stdlib.h>		// free
#include <sys/stat.h>	// struct stat

#include "ft_ls.h"

void	init(struct s_list *head)
{
	head->prev = head;
	head->next = head;
}

/*
void	insert_after(struct s_list *item, struct s_list *new)
{
	new->prev = item;
	new->next = item->next;
	item->next->prev = new;
	item->next = new;
}
*/

/*
void	delete(struct s_list *elem)
{
	elem->prev->next = elem->next;
	elem->next->prev = elem->prev;
	if (elem->name)
		free(elem->name);
	if (elem->link)
		free(elem->link);
	if (elem->uname)
		free(elem->uname);
	if (elem->gname)
		free(elem->gname);
	free(elem);
}
*/
