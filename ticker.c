#include "wax.h"
#include <ncursesw/menu.h>

#define TICKER_HEIGHT (LINES - 4)

int SONG_CURRTIME = 0;
int SONG_DUR = 0;
int TICKER_START;
int TICKER_END;
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

void drawTicker() {
  TICKER_START = (COLS / 14) + 5;
  TICKER_END = COLS - (TICKER_START * 2) - 1;
  TICKER_LENGTH = TICKER_END - TICKER_START;

  wattron(stdscr, COLOR_PAIR(1));
  mvhline(TICKER_HEIGHT, TICKER_START, ACS_VLINE, TICKER_END);
  wattroff(stdscr, COLOR_PAIR(1));
  mvhline(TICKER_HEIGHT, TICKER_START, ACS_CKBOARD, PLAYED_LENGTH());
  refresh();
}
