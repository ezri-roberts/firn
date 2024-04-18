#include "menu.h"
#include <stdio.h>

fmenu fmenu_new() {

	fmenu menu;

	menu.running = true;
	menu.selected = 0;
	menu.working = fdir_new(".");

	tcgetattr(STDIN_FILENO, &menu.oldt); // save the terminal attributes
	menu.newt = menu.oldt; // copy the old settings
	menu.newt.c_lflag &= ~(ICANON | ECHO); // disable canonical mode and echoing
	tcsetattr(STDIN_FILENO, TCSANOW, &menu.newt); // apply new settings
	_cursor_enable(false);

	sprintf(menu.user, "%s@%s", getenv("USER"), getenv("HOSTNAME"));

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

	_print(BK_BLACK, FG_GREEN, "%s\n", menu->user);

	for (size_t i = 0; i < menu->working.files.used; i++) {

		struct dirent *item = menu->working.files.items[i];

		bool selected = (menu->selected == i);
		const char *bk;
		const char *fg;

		if (item->d_type == 4) {

			bk = selected ? BK_BLUE : BK_BLACK;
			fg = selected ? FG_BLACK : FG_BLUE;
		} else {

			bk = selected ? BK_WHITE : BK_BLACK;
			fg = selected ? FG_BLACK : FG_WHITE;
		}


		_print(bk, fg, "%s\n", item->d_name);
	}

	fmenu_input(menu);
	fmenu_update(menu);
}

void _print(const char *bk, const char *fg, const char *format, ...) {

	char color[128];
	sprintf(color, "\x1b[%s;%sm", bk, fg);

	char str[256];
	sprintf(str, "%s%s%s", color, format, COLOR_RESET);

	va_list argptr;
	va_start(argptr, format);
	vprintf(str, argptr);
	va_end(argptr);
}

void _cursor_enable(bool enable) {
	enable ? printf("\e[?25h") : printf("\e[?25l");
}

void _clear() {
	printf("\e[1;1H\e[2J");
}

void fmenu_destroy(fmenu *menu) {

	_clear();
	_cursor_enable(true);
	fdir_destroy(&menu->working);
	tcsetattr(STDIN_FILENO, TCSANOW, &menu->oldt); // restore the original terminal attributes.
	printf("Closing.\n");
}
