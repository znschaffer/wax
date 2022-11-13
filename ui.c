#include "wax.h"
#include <stdlib.h>
#include <string.h>

ITEM **song_items;
MENU *song_menu;
WINDOW *song_win;
char *title;

void setupCurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
}

int setupUI() {
  int ch;
  setupCurses();
  setupColors();
  song_items = (ITEM **)calloc(200, sizeof(ITEM *));
  populateSongItems();
  song_menu = new_menu((ITEM **)song_items);

  song_win = newwin(0, 0, 0, 0);
  keypad(song_win, TRUE);

  set_menu_win(song_menu, song_win);
  set_menu_sub(song_menu, derwin(song_win, n_songs, COLS - 10, 3, 3));

  set_menu_mark(song_menu, " > ");

  /* Print a border around the main window and print a title */
  box(song_win, 0, 0);
  title = "wax";

  // print_in_middle(song_win, 1, (COLS / 2) - (strlen(title) / 2),
  // strlen(title),
  //                 title, 1);
  mvwhline(song_win, 2, 1, ACS_HLINE, COLS - 2);
  refresh();

  post_menu(song_menu);
  wrefresh(song_win);

  while ((ch = getch()) != KEY_F(1)) {
    mvprintw(LINES - 3, 0, "%d", getSongTime());
    handleInput(ch);
  }
  return 0;
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
    move(1, 1);
    clrtoeol();
    printMiddle(song_win, 1,
                (COLS / 2) -
                    (strlen(getCurrTitle(song_menu->curitem->index)) / 2),
                strlen(getCurrTitle(song_menu->curitem->index)),
                getCurrTitle(song_menu->curitem->index), 1);
    refresh();
    loadSound(song_menu->curitem);
    break;
  }
  wrefresh(song_win);
}

void cleanupUI() { endwin(); }
