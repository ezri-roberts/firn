#include "list.h"

fitem* fitem_new(const char root[PATH_MAX], const char name[256]) {

	fitem *item = malloc(sizeof(fitem));
	item->name = (char*)name;
	// item->name = malloc(256 * sizeof(char));
	// memcpy(item->name, name, 256 * sizeof(char));
	item->path = malloc(PATH_MAX * sizeof(char));
	sprintf(item->path, "%s/%s", root, name);

	struct stat item_info;
	stat(item->path, &item_info);

	item->size = item_info.st_size;

	return item;
}

fitem_list fitem_list_new() {

	fitem_list list;

	list.items = malloc(ITEM_LIST_SIZE * sizeof(fitem));
	list.used = 0;
	list.size = ITEM_LIST_SIZE;

	return list;
}

void fitem_list_insert(fitem_list *list, fitem *item) {

	if (list->used == list->size) {
		list->size *= 2;
		list->items = realloc(list->items, list->size * sizeof(fitem));
	}

	list->items[list->used++] = item;
}

void fitem_list_destroy(fitem_list *list) {

	for (int i = 0; i < list->used; i++) {

		free(list->items[i]->path);
		free(list->items[i]);
		list->items[i] = NULL;
	}

	free(list->items);
	list->items = NULL;
	list->used = 0;
	list->size = 0;
}
