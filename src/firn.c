#include "firn.h"

firn firn_new(const char *path) {

	firn inst;

	inst.running = true;
	inst.selected = 0;
	inst.working = fdir_new(path);
	inst.down = NULL;

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


	int limit = inst->working->dirs.used + inst->working->files.used;

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

		case 'l':
			if (inst->current->type == 4 && inst->current->readable && inst->current->size > 0) {
				inst->working = fdir_new(inst->down->path);
				inst->selected = 0;
			}
			break;

		case 'h':
			if (strcmp(inst->working->path, "/") != 0) {
				char parent[PATH_MAX];
				sprintf(parent, "%s/..", inst->working->path);
				inst->working = fdir_new(parent);
				inst->selected = 0;
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

	if (inst->selected >= inst->working->dirs.used) {
		inst->current = inst->working->files.items[
			inst->selected - inst->working->dirs.used
		];
	} else {
		inst->current = inst->working->dirs.items[inst->selected];
	}

	_clear();

	_print(BK_BLACK, FG_GREEN, false, "%s ", inst->user);
	_print(BK_BLACK, FG_WHITE, false, "%s", inst->working->path);

	firn_display_list(inst, &inst->working->dirs, true, 0);
	firn_display_list(inst, &inst->working->files, true, inst->working->dirs.used);

	if (inst->current->type == 4) {

		firn_display_list(inst, &inst->down->dirs, false, 0);
		firn_display_list(inst, &inst->down->files, false, inst->down->dirs.used);
	}

	firn_input(inst);
	firn_update(inst);
}

void firn_display_list(firn *inst, fitem_list *list, bool active, int offset) {

	for (size_t i = 0; i < list->used; i++) {

		fitem *item = list->items[i];

		bool over = false;

		const char *bk = item->bk_color;
		const char *fg = item->fg_color;

		if (active) {

			over = ((inst->selected - offset) == i);

			if (item->selected) {
				fg = FG_GREEN;
			}

			if (over && item->type == 4) {
				inst->down = fdir_new(item->path);
			}
		}

		if (!item->readable) {
			fg = FG_RED;
		}

		char name[50];
		char prefix = item->selected ? '*' : ' ';
		unsigned long spacing;
		char *fmt;
		char *mem_str = _get_mem(item->size);

		if (strlen(item->name) > 50) { // Cutoff display of file names that are too long.

			// char name_sub[35];
			memcpy(name, &item->name[0], 49);
			name[49] = '\0';

			spacing = 60 - (49 + 3);
			fmt = active ? "\n%c%s...%*s" : "%c%s...%*s";
		} else {

			memcpy(name, item->name, 49);
			spacing = 60 - strlen(item->name);
			fmt = active ? "\n%c%s%*s" : "%c%s%*s";
		}

		if (active) {
			_print(bk, fg, over, fmt, prefix, name, spacing, mem_str);
		} else {
			_print_off((int)(i+2 + offset), 63, bk, fg, over, fmt, prefix, name, spacing, mem_str);
		}

		free(mem_str);

	}
}

char* _get_mem(long size) {

	double num = (double)size;
	char *symbol = "B";

	if (size >= pow(10, 9)) {
		symbol = "GB";
		num = (double)size / pow(10, 9);
	} else if (size >= pow(10, 6)) {
		symbol = "MB";
		num = (double)size / pow(10, 6);
	} else if (size >= 1000) {
		symbol = "KB";
		num = (double)size / 1000;
	}

	char *str = malloc(256 * sizeof(char));
	sprintf(str, "%0.1f%s", num, symbol);

	return str;
}

void _print(const char *bk, const char *fg, bool reversed, const char *format, ...) {

	int rev = reversed ? 7 : 0;

	char color[128];
	sprintf(color, "\x1b[%d;%s;%sm", rev, bk, fg);

	char str[1024];
	sprintf(str, "%s%s%s", color, format, COLOR_RESET);

	va_list argptr;
	va_start(argptr, format);
	vprintf(str, argptr);
	va_end(argptr);
}

void _print_off(int yoff, int xoff, const char *bk, const char *fg, bool reversed, const char *format, ...) {

	int rev = reversed ? 7 : 0;

	char offset[128];
	sprintf(offset, "\033[%d;%dH", yoff, xoff);
	char color[128];
	sprintf(color, "\x1b[%d;%s;%sm", rev, bk, fg);

	char str[1024];
	sprintf(str, "%s%s%s%s", offset, color, format, COLOR_RESET);

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
	fdir_destroy(inst->working);
	fdir_destroy(inst->down);
	tcsetattr(STDIN_FILENO, TCSANOW, &inst->oldt); // restore the original terminal attributes.
	printf("Closing.\n");
}
