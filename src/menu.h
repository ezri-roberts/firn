#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include "dir.h"

#define COLOR_RESET "\x1B[0m"
#define COLOR_RED  "\x1B[31m"
#define COLOR_GREEN  "\x1B[32m"
#define COLOR_YELLLOW  "\x1B[33m"
#define COLOR_BLUE  "\x1B[34m"
#define COLOR_MAGENTA  "\x1B[35m"
#define COLOR_CYNAN  "\x1B[36m"
#define COLOR_WHTITE  "\x1B[37m"

typedef struct {

	fdir working;
	fdir up;
	fdir down;

	int selected;
	bool running;

	struct termios oldt; // Original terminal attributes.
	struct termios newt; // Our modified attributes.

} fmenu;

fmenu fmenu_new();
void fmenu_input(fmenu *menu);
void fmenu_update(fmenu *menu);
void fmenu_destroy(fmenu *menu);
// Print out colored text.
void _print(const char *color, const char *format, ...);
void _clear();

#endif // !MENU_H
