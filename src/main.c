#include "firn.h"

int main(int argc, char *argv[]) {

	(void)argc;

	char *path = argv[1] ? argv[1] : ".";
	firn inst = firn_new(path);

	firn_update(&inst);
	firn_destroy(&inst);

	return 0;
}
