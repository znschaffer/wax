#include "wax.h"

typedef struct {
  char key;
  char *text;
} Hotkey;

#define P_PAUSE "pause"
#define P_PLAY "play"

Hotkey hotkeys[6] = {
    {.key = '[', .text = "backward"},   {.key = ']', .text = "forward"},
    {.key = ',', .text = "prev"},       {.key = '.', .text = "next"},
    {.key = 'p', .text = "play"}, {.key = 'q', .text = "quit"}};

void drawHotkeyBar() {

    if (isPlaying()) {
        hotkeys[4].text = P_PAUSE;
    } else {
        hotkeys[4].text = P_PLAY;
    }

  int colBuffer = COLS / 10;
  int i;
  if ((LINES < 10) || (COLS < 75)) {
    return;
  }

  for (i = 0; i < 6; i++) {
    wattron(stdscr, COLOR_PAIR(3));
    mvprintw(LINES - 2, ((((COLS - colBuffer) / 6)) * i) + colBuffer, " %c ",
             hotkeys[i].key);
    wattron(stdscr, COLOR_PAIR(2));
    mvprintw(LINES - 2, ((((COLS - colBuffer) / 6)) * i) + colBuffer + 3,
             " %s ", hotkeys[i].text);
    wattroff(stdscr, COLOR_PAIR(2));
  }
}
