#ifndef LIST_H
#define LIST_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/limits.h>

#define ITEM_LIST_SIZE 128

typedef struct {

	char *path;
	char *name;
	int type;
	off_t size;
	bool selected;

} fitem;

typedef struct {

	fitem **items;
	size_t size;
	size_t used;

} fitem_list;

fitem* fitem_new(const char root[PATH_MAX], const char name[256]);
fitem_list fitem_list_new();
void fitem_list_insert(fitem_list *list, fitem *item);
void fitem_list_destroy(fitem_list *list);

#endif // !LIST_H
