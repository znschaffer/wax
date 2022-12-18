#include "wax.h"
#include <ncursesw/menu.h>

int SONG_CURRTIME = 0;
int SONG_DUR = 0;
int TICKER_START = 8;
int TICKER_END;
int TICKER_HEIGHT;
int TICKER_LENGTH;

float percentPlayed() {
  if (soundInitialized) {
    return (float)(SONG_CURRTIME) / (float)(SONG_DUR);
  } else {
    return 0;
  }
}
int PLAYED_LENGTH() { return (int)(percentPlayed() * TICKER_LENGTH); }

void drawTime(WINDOW *win) {}

void drawTicker(WINDOW *win) {
  TICKER_END = COLS - 17;
  TICKER_HEIGHT = LINES - 4;
  TICKER_LENGTH = TICKER_END - TICKER_START;

  mvwhline(win, TICKER_HEIGHT, TICKER_START, ACS_VLINE, TICKER_END);
  mvwhline(win, TICKER_HEIGHT, TICKER_START, ACS_CKBOARD, PLAYED_LENGTH());
  refresh();
}
