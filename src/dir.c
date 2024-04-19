#include "dir.h"

fdir fdir_new(const char *path) {

	fdir dir;

	dir.path = malloc(PATH_MAX * sizeof(char));
	realpath(path, dir.path);

	dir.files = fitem_list_new();

	DIR *d;
  struct dirent *dirent_item;
  d = opendir(path);

  if (d) {

    while ((dirent_item = readdir(d)) != NULL) {

			if (dirent_item->d_name[0] == '.') continue;

			fitem *item = fitem_new(dir.path, dirent_item);
			fitem_list_insert(&dir.files, item);
    }

    closedir(d);
  }

	return dir;
}

void fdir_destroy(fdir *dir) {

	free(dir->path);
	fitem_list_destroy(&dir->files);
}
