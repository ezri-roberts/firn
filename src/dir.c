#include "dir.h"

fdir fdir_new(const char *path) {

	fdir dir;

	dir.path = malloc(PATH_MAX * sizeof(char));
	realpath(path, dir.path);

	dir.files = item_list_new();

	DIR *d;
  struct dirent *item;
  d = opendir(path);

  if (d) {

    while ((item = readdir(d)) != NULL) {

			if (item->d_name[0] == '.') continue;

			item_list_insert(&dir.files, item);
    }

    closedir(d);
  }

	return dir;
}

void fdir_destroy(fdir *dir) {

	free(dir->path);
	item_list_destroy(&dir->files);
}
