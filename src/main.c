#include "firn.h"

int main(int argc, char *argv[]) {

	(void)argc; (void)argv;

	firn inst = firn_new();

	firn_update(&inst);
	firn_destroy(&inst);

	return 0;
}
