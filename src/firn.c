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

	inst->current = inst->working.files.items[inst->selected];

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

		case ' ':
			inst->current->selected = !inst->current->selected;
			if (inst->selected < inst->working.files.used-1) {
				inst->selected++;
			}
			break;

		case 'q':
			inst->running = false;
	}

}

void firn_update(firn *inst) {

	if (!inst->running) return;

	_clear();

	_print(BK_BLACK, FG_GREEN, "%s ", inst->user);
	_print(BK_BLACK, FG_WHITE, "%s\n", inst->working.path);

	for (size_t i = 0; i < inst->working.files.used; i++) {

		fitem *item = inst->working.files.items[i];

		bool selected = (inst->selected == i);
		const char *bk;
		const char *fg;

		if (item->type == 4) {

			bk = selected ? BK_BLUE : BK_BLACK;
			fg = selected ? FG_BLACK : FG_BLUE;
		} else {

			bk = selected ? BK_WHITE : BK_BLACK;
			fg = selected ? FG_BLACK : FG_WHITE;
		}

		if (item->selected) {

			bk = selected ? BK_GREEN : BK_BLACK;
			fg = selected ? FG_BLACK : FG_GREEN;
		}

		char prefix = item->selected ? '*' : ' ';

		if (strlen(item->name) > 55) { // Cutoff display of file names that are too long.

			char name_sub[55];
			memcpy(name_sub, &item->name[0], 54);
			name_sub[54] = '\0';

			unsigned long spacing = 60 - (strlen(name_sub) + 3);
			_print(bk, fg, "%c%s...%*luB\n", prefix, name_sub, spacing, item->size);
		} else {

			unsigned long spacing = 60 - strlen(item->name);
			_print(bk, fg, "%c%s%*luB\n", prefix, item->name, spacing, item->size);
		}

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
