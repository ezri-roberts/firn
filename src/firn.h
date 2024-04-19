#ifndef FIRN_H
#define FIRN_H

#include <stdbool.h>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include "dir.h"

typedef struct {

	fdir working;
	fdir down;

	// user + hostname
	char user[512];		 // Linux supports user and hostnames of up to 255 bytes.
	int selected;
	fitem *current;
	bool running;

	struct termios oldt; // Original terminal attributes.
	struct termios newt; // Our modified attributes.

} firn;

firn firn_new(const char *path);
void firn_input(firn *inst);
void firn_update(firn *inst);
void firn_display_list(firn *inst, fitem_list *list, bool active, int offset);
void firn_destroy(firn *inst);
// Print out colored text.
void _print(const char *bk, const char *fg, bool reversed, const char *format, ...);
void _print_off(int yoff, int xoff, const char *bk, const char *fg, bool reversed, const char *format, ...);
// Show or hide the cursor.
void _cursor_enable(bool enable);
void _clear();

#endif // !FIRN_H
