
#include "wax.h"

char *getCurrTitle(short index) {

  char *str[100];

  strcpy(str, "Now Playing: ");
  strcat(str, song_items[index]->name.str);
  strcat(str, " - ");
  strcat(str, song_items[index]->description.str);

  return str;
}

void printMiddle(WINDOW *win, int starty, int startx, int width, char *string,
                 chtype color) {
  int length, x, y;
  float temp;

  if (win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if (startx != 0)
    x = startx;
  if (starty != 0)
    y = starty;
  if (width == 0)
    width = 80;

  length = strlen(string);
  temp = (width - length) / 2;
  x = startx + (int)temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}