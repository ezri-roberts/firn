#ifndef FIRN_H
#define FIRN_H

#include <stdbool.h>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include "dir.h"

#define COLOR_RESET "\x1B[0m"
#define FG_BLACK "30"
#define FG_RED  "31"
#define FG_GREEN  "32"
#define FG_YELLLOW  "33"
#define FG_BLUE  "34"
#define FG_MAGENTA  "35"
#define FG_CYNAN  "36"
#define FG_WHITE  "37"
#define BK_BLACK "40"
#define BK_RED  "41"
#define BK_GREEN  "42"
#define BK_YELLLOW  "43"
#define BK_BLUE  "44"
#define BK_MAGENTA  "45"
#define BK_CYNAN  "46"
#define BK_WHITE  "47"

typedef struct {

	fdir working;
	fdir down;

	// user + hostname
	char user[512];		 // Linux supports user and hostnames of up to 255 bytes.
	int selected;
	bool running;

	struct termios oldt; // Original terminal attributes.
	struct termios newt; // Our modified attributes.

} firn;

firn firn_new();
void firn_input(firn *inst);
void firn_update(firn *inst);
void firn_destroy(firn *inst);
// Print out colored text.
void _print(const char *bk, const char *fg, const char *format, ...);
// Show or hide the cursor.
void _cursor_enable(bool enable);
void _clear();

#endif // !FIRN_H
