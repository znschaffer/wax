#include "wax.h"

char title[200];

char *getCurrTitle(short index) {

  // char *str = calloc(song_items[index]->name.length +
  //                        song_items[index]->description.length + 3,
  //                    sizeof(char));
  memset(&title[0], 0, sizeof(title));
  strcpy(title, "Now Playing: ");
  strcat(title, song_items[index]->name.str);
  strcat(title, " - ");
  strcat(title, song_items[index]->description.str);
  return title;
}

void printMiddle(WINDOW *win, int starty, int startx, int width, char *string,
                 chtype color) {
  int length, x, y;
  int temp;

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
  x = startx + temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}
