#include "firn.h"

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

	if (inst->selected >= inst->working.dirs.used) {
		inst->current = inst->working.files.items[
			inst->selected - inst->working.dirs.used
		];
	} else {
		inst->current = inst->working.dirs.items[inst->selected];
	}

	int limit = inst->working.dirs.used + inst->working.files.used;

	switch (ch) {

		case 'j': 
			if (inst->selected < limit-1) {
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
			if (inst->selected < limit-1) {
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

	_print(BK_BLACK, FG_GREEN, false, "%s ", inst->user);
	_print(BK_BLACK, FG_WHITE, false, "%s\n", inst->working.path);

	firn_display_list(inst, &inst->working.dirs, 0);
	firn_display_list(inst, &inst->working.files, inst->working.dirs.used);

	firn_input(inst);
	firn_update(inst);
}

void firn_display_list(firn *inst, fitem_list *list, int offset) {

	for (size_t i = 0; i < list->used; i++) {

		fitem *item = list->items[i];

		bool selected = ((inst->selected - offset) == i);

		const char *bk = item->bk_color;
		const char *fg = item->fg_color;

		if (item->selected) {
			fg = FG_GREEN;
		}

		char prefix = item->selected ? '*' : ' ';

		if (strlen(item->name) > 55) { // Cutoff display of file names that are too long.

			char name_sub[55];
			memcpy(name_sub, &item->name[0], 54);
			name_sub[54] = '\0';

			unsigned long spacing = 60 - (strlen(name_sub) + 3);
			_print(bk, fg, selected, "%c%s...%*luB\n", prefix, name_sub, spacing, item->size);
		} else {

			unsigned long spacing = 60 - strlen(item->name);
			_print(bk, fg, selected, "%c%s%*luB\n", prefix, item->name, spacing, item->size);
		}

	}
}

void _print(const char *bk, const char *fg, bool reversed, const char *format, ...) {

	int rev = reversed ? 7 : 0;

	char color[128];
	sprintf(color, "\x1b[%d;%s;%sm", rev, bk, fg);

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
