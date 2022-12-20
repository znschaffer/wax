#include "wax.h"

typedef struct {
  char key;
  char text[30];
} Hotkey;

Hotkey hotkeys[6] = {
    {.key = '[', .text = "rw"},   {.key = ']', .text = "ff"},
    {.key = ',', .text = "prev"}, {.key = '.', .text = "next"},
    {.key = 'p', .text = "play"}, {.key = 'q', .text = "quit"}};

void drawHotkeyBar() {
  int colBuffer = COLS / 10;
  int i;
  if ((LINES < 10) || (COLS < 75)) {
    return;
  }

  for (i = 0; i < 6; i++) {
    wattron(stdscr, COLOR_PAIR(2));
    mvprintw(LINES - 2, ((((COLS - colBuffer) / 6)) * i) + colBuffer, " %c ",
             hotkeys[i].key);
    wattron(stdscr, COLOR_PAIR(1));
    mvprintw(LINES - 2, ((((COLS - colBuffer) / 6)) * i) + colBuffer + 3,
             " %s ", hotkeys[i].text);
    wattroff(stdscr, COLOR_PAIR(1));
  }
}
