#ifndef DIR_H
#define DIR_H

#include "list.h"

typedef struct {

	const char *path;
	item_list files;

} fdir;

fdir fdir_new(const char *path);
void fdir_destroy(fdir *dir);

#endif // !DIR_H
