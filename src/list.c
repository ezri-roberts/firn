#include "list.h"

item_list item_list_new() {

	item_list list;

	list.items = malloc(ITEM_LIST_SIZE * sizeof(struct dirent));
	list.used = 0;
	list.size = ITEM_LIST_SIZE;

	return list;
}

void item_list_insert(item_list *list, struct dirent *item) {

	if (list->used == list->size) {
		list->size *= 2;
		list->items = realloc(list->items, list->size * sizeof(struct dirent));
	}

	list->items[list->used++] = item;
}

void item_list_destroy(item_list *list) {

	for (size_t i = 0; i < list->used; i++) {

		struct dirent *item = list->items[i];

		free(item);
		item = NULL;
	}

	free(list->items);
	list->items = NULL;
	list->used = 0;
	list->size = 0;
}
