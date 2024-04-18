#include "dir.h"

fdir dir_new(const char *path) {

	fdir dir;

	dir.path = path;
	dir.files = item_list_new();

	DIR *d;
  struct dirent *item;
  d = opendir(".");

  if (d) {

    while ((item = readdir(d)) != NULL) {

			if (item->d_name[0] == '.') continue;

      printf("%s\n", item->d_name);
			item_list_insert(&dir.files, item);
    }

    closedir(d);
  }

	return dir;
}
