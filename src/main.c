#include "dir.h"
#include "../lib/termbox2/termbox2.h"

int main(int argc, char *argv[]) {

	(void)argc; (void)argv;

	fdir dir = dir_new(".");

	tb_init();

	tb_shutdown();

	return 0;
}
