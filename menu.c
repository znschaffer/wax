#include "wax.h"

char title[200];

char *getCurrTitle(short index) {
  memset(&title[0], 0, sizeof(title));
  if (isPlaying()) {
    strcpy(title, "Now Playing: ");
    strcat(title, currArtist);
    strcat(title, " - ");
    strcat(title, song_items[index]->description.str);
  } else {

    strcpy(title, "Paused: ");
    strcat(title, currArtist);
    strcat(title, " - ");
    strcat(title, song_items[index]->description.str);
  }
  return title;
}

void printMiddle(WINDOW *win, int starty, int startx, int width, char *string,
                 chtype color) {
  int length, x, y;
  int temp;

  if (NULL == win)
    win = stdscr;
  getyx(win, y, x);
  if (0 != startx)
    x = startx;
  if (0 != starty)
    y = starty;
  if (0 == width)
    width = 80;

  length = strlen(string);
  temp = (width - length) / 2;
  x = startx + temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}
