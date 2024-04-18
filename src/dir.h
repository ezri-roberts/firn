#ifndef DIR_H
#define DIR_H

#include "list.h"

typedef struct {

	const char *path;
	item_list files;

} fdir;

fdir dir_new(const char *path);

#endif // !DIR_H
