#include "wax.h"

typedef struct {
  char key;
  char text[30];
} Hotkey;

Hotkey hotkeys[6] = {
    {.key = '[', .text = "rewind"},     {.key = ']', .text = "fforward"},
    {.key = ',', .text = "prev"},       {.key = '.', .text = "next"},
    {.key = 'p', .text = "play/pause"}, {.key = 'q', .text = "quit"}};

void drawHotkeyBar() {
  int i;
  for (i = 0; i < 6; i++) {
    wattron(stdscr, COLOR_PAIR(2));
    mvprintw(LINES - 2, ((COLS / 5.6) * i) + 2, " %c ", hotkeys[i].key);
    wattron(stdscr, COLOR_PAIR(1));
    mvprintw(LINES - 2, ((COLS / 5.6) * i) + 5, " %s ", hotkeys[i].text);
    wattroff(stdscr, COLOR_PAIR(1));
  }
}