#include "wax.h"
#include <stdlib.h>

char file_paths[200][100]; // TODO: find a better way to do this
ITEM **songs;
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
  songs = (ITEM **)calloc(n_songs + 1, sizeof(ITEM *));
  loadSongs();
  song_menu = new_menu((ITEM **)songs);
  post_menu(song_menu);
  refresh();

  while ((ch = getch()) != KEY_F(1)) {
    mvprintw(LINES-3, 0, "%d", getSongTime());
    handleInput(ch);
  }
}

void loadDirectory(char *dirPath) { ftw(dirPath, addSongsToList, 7); }

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
    loadSound((char *)song_menu->curitem->name.str);
    break;
  }
}
