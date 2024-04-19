#include "list.h"

fitem* fitem_new(const char root[PATH_MAX], struct dirent *data) {

	fitem *item = malloc(sizeof(fitem));

	item->selected = false;
	item->type = data->d_type;

	item->name = malloc(strlen(data->d_name) + strlen("/") + 1);
	strcpy(item->name, data->d_name);

	if (item->type == 4) {

		strcat(item->name, "/");

		item->bk_color = BK_BLACK;
		item->fg_color = FG_BLUE;
	} else {

		item->bk_color = BK_BLACK;
		item->fg_color = FG_WHITE;
	}

	item->path = malloc(PATH_MAX * sizeof(char));
	sprintf(item->path, "%s/%s", root, item->name);

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
		free(list->items[i]->name);
		free(list->items[i]);
		list->items[i] = NULL;
	}

	free(list->items);
	list->items = NULL;
	list->used = 0;
	list->size = 0;
}
