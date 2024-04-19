#ifndef DIR_H
#define DIR_H

#include "list.h"
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

typedef struct {

	char *path;
	fitem_list files;

} fdir;

fdir fdir_new(const char *path);
void fdir_destroy(fdir *dir);

#endif // !DIR_H
