#ifndef LIST_H
#define LIST_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#define ITEM_LIST_SIZE 128

typedef struct {

	struct dirent **items;
	size_t size;
	size_t used;

} item_list;

item_list item_list_new();
void item_list_insert(item_list *list, struct dirent *item);
void item_list_destroy(item_list *list);

#endif // !LIST_H
