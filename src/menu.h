#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <string.h>
#include "dir.h"

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
void _clear();

#endif // !MENU_H
