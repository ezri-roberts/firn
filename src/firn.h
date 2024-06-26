#ifndef FIRN_H
#define FIRN_H

#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "dir.h"

typedef struct {

	fdir *working;
	fdir *down;

	// user + hostname
	char user[NAME_MAX * 2];
	int selected;
	int action;				 // 0 = none, 1 = delete, 2 = copy.
	fitem *current;
	bool running;

	struct termios oldt; // Original terminal attributes.
	struct termios newt; // Our modified attributes.

} firn;

firn firn_new(const char *path);
void firn_input(firn *inst);
bool firn_confirm(firn *inst);
void firn_update(firn *inst);
void firn_display_list(firn *inst, fitem_list *list, bool active, int offset);
void firn_delete_item(firn *inst);
void firn_yank(firn *inst);
void firn_put(firn *inst);
void firn_destroy(firn *inst);
// Print out colored text.
void _print(const char *bk, const char *fg, bool reversed, const char *format, ...);
void _print_off(int yoff, int xoff, const char *bk, const char *fg, bool reversed, const char *format, ...);
int _get_selected(firn *inst, fitem_list *list);
char* _get_mem(long size);
// Show or hide the cursor.
void _cursor_enable(bool enable);
void _clear();
long clamp(long d, long min, long max);

#endif // !FIRN_H
