#ifndef DIR_H
#define DIR_H

#include <dirent.h>
#include <stdio.h>
#include "list.h"

typedef struct {

	const char *path;
	item_list files;

} directory;

directory dir_new(const char *path);

#endif // !DIR_H
