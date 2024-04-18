#include "menu.h"

fmenu fmenu_new() {

	fmenu menu;

	menu.running = true;
	menu.selected = 0;
	menu.working = fdir_new(".");

	tcgetattr(STDIN_FILENO, &menu.oldt); // save the terminal attributes
	menu.newt = menu.oldt; // copy the old settings
	menu.newt.c_lflag &= ~(ICANON | ECHO); // disable canonical mode and echoing
	tcsetattr(STDIN_FILENO, TCSANOW, &menu.newt); // apply new settings

	return menu;
}

void fmenu_input(fmenu *menu) {

	int ch;
	ch = getchar();

	switch (ch) {
		case 'j': 
			if (menu->selected < menu->working.files.used-1) {
				menu->selected++;
			}
			break;
		case 'k':
			if (menu->selected > 0) {
				menu->selected--;
			}
			break;
		case 'q':
			menu->running = false;
	}

}

void fmenu_update(fmenu *menu) {

	if (!menu->running) return;

	_clear();

	for (size_t i = 0; i < menu->working.files.used; i++) {

		struct dirent *item = menu->working.files.items[i];

		if (i == menu->selected) printf("> ");
		printf("%s\n", item->d_name);
	}

	fmenu_input(menu);
	fmenu_update(menu);
}

void _clear() {
	printf("\e[1;1H\e[2J");
}

void fmenu_destroy(fmenu *menu) {

	_clear();
	fdir_destroy(&menu->working);
	tcsetattr(STDIN_FILENO, TCSANOW, &menu->oldt); // restore the original terminal attributes.
	printf("Closing.\n");
}