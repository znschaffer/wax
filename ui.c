#include "wax.h"
#include <stdlib.h>

ITEM **song_items;
MENU *song_menu;
int n_songs = 0;

void setupCurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
}

void setupUI() {
  int ch;
  setupCurses();
  song_items = (ITEM **)calloc(200, sizeof(ITEM *));
  populateSongItems();
  song_menu = new_menu((ITEM **)song_items);
  post_menu(song_menu);
  refresh();

  while ((ch = getch()) != KEY_F(1)) {
    mvprintw(LINES-3, 0, "%d", getSongTime());
    handleInput(ch);
  }
}

void handleInput(int ch) {
  switch (ch) {
  case 'j':
  case KEY_DOWN:
    menu_driver(song_menu, REQ_DOWN_ITEM);
    break;
  case 'k':
  case KEY_UP:
    menu_driver(song_menu, REQ_UP_ITEM);
    break;
  case 'p':
    mvprintw(LINES - 2, 0, "loading song from %s\n",
             song_menu->curitem->name.str);
    refresh();
    loadSound(song_menu->curitem);
    break;
  }
}

void cleanupUI() {
  endwin();
}
