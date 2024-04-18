#include "firn.h"
#include <stdio.h>

firn firn_new() {

	firn inst;

	inst.running = true;
	inst.selected = 0;
	inst.working = fdir_new(".");

	tcgetattr(STDIN_FILENO, &inst.oldt); // save the terminal attributes
	inst.newt = inst.oldt; // copy the old settings
	inst.newt.c_lflag &= ~(ICANON | ECHO); // disable canonical mode and echoing
	tcsetattr(STDIN_FILENO, TCSANOW, &inst.newt); // apply new settings
	_cursor_enable(false);

	sprintf(inst.user, "%s@%s", getenv("USER"), getenv("HOSTNAME"));

	return inst;
}

void firn_input(firn *inst) {

	int ch;
	ch = getchar();

	switch (ch) {
		case 'j': 
			if (inst->selected < inst->working.files.used-1) {
				inst->selected++;
			}
			break;
		case 'k':
			if (inst->selected > 0) {
				inst->selected--;
			}
			break;
		case 'q':
			inst->running = false;
	}

}

void firn_update(firn *inst) {

	if (!inst->running) return;

	_clear();

	_print(BK_BLACK, FG_GREEN, "%s\n", inst->user);

	for (size_t i = 0; i < inst->working.files.used; i++) {

		struct dirent *item = inst->working.files.items[i];

		bool selected = (inst->selected == i);
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

	firn_input(inst);
	firn_update(inst);
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

void firn_destroy(firn *inst) {

	_clear();
	_cursor_enable(true);
	fdir_destroy(&inst->working);
	tcsetattr(STDIN_FILENO, TCSANOW, &inst->oldt); // restore the original terminal attributes.
	printf("Closing.\n");
}
