#include "menu.h"

int main(int argc, char *argv[]) {

	(void)argc; (void)argv;

	fmenu menu = fmenu_new();

	fmenu_update(&menu);
	fmenu_destroy(&menu);

	return 0;
}
