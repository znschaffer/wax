#include "wax.h"

ITEM **song_items;
MENU *song_menu;
WINDOW *song_win;
char *title;
int n_songs = 0;

void setupCurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
}

void logSongItems(void) {
  for (int i = 0; i < n_songs; i++) {
    fprintf(log_file, "n_songs:%d:%d\n", i, song_items[i]->index);
  }
}

int setupUI() {
  int ch;
  setupCurses();
  setupColors();
  song_items = (ITEM **)calloc(400, sizeof(ITEM *));
  populateSongItems();
  song_menu = new_menu((ITEM **)song_items);

  /* Init windows */
  song_win = newwin(0, 0, 0, 0);
  keypad(song_win, TRUE);

  set_menu_win(song_menu, song_win);
  set_menu_sub(song_menu, derwin(song_win, LINES - 10, COLS - 10, 4, 2));
  set_menu_format(song_menu, LINES - 15, 1);
  set_menu_mark(song_menu, " > ");

  /* Print a border around the main window and print a title */
  box(song_win, 0, 0);
  title = "wax";

  printMiddle(song_win, 1, (COLS / 2) - (strlen(title) / 2), strlen(title),
              title, 1);
  mvwhline(song_win, 2, 1, ACS_HLINE, COLS - 2);
  mvwhline(song_win, LINES - 10, 1, ACS_HLINE, COLS - 2);
  mvwvline(song_win, LINES - 9, COLS - COLS / 4, ACS_VLINE, 8);
  refresh();
  post_menu(song_menu);
  wrefresh(song_win);

  while ((ch = getch()) != 'q') {
    int currSecs = getSongTime();
    mvprintw(1, COLS - 10, "%d:%d", (currSecs/60)%60, currSecs%60);
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
  case '>':
    menu_driver(song_menu, REQ_DOWN_ITEM);
    for (int i = 1; i < COLS - 1; i++) {
      mvprintw(1, i, " ");
    }
    printMiddle(song_win, 1,
                (COLS / 2) -
                    (strlen(getCurrTitle(song_menu->curitem->index)) / 2),
                strlen(getCurrTitle(song_menu->curitem->index)),
                getCurrTitle(song_menu->curitem->index), 1);
    refresh();
    nextSong();
    break;
  case '<':
    menu_driver(song_menu, REQ_UP_ITEM);
    for (int i = 1; i < COLS - 1; i++) {
      mvprintw(1, i, " ");
    }
    printMiddle(song_win, 1,
                (COLS / 2) -
                    (strlen(getCurrTitle(song_menu->curitem->index)) / 2),
                strlen(getCurrTitle(song_menu->curitem->index)),
                getCurrTitle(song_menu->curitem->index), 1);
    refresh();
    prevSong();
    break;
  case ' ':
    toggleSong();
    for (int i = 1; i < COLS - 1; i++) {
      mvprintw(1, i, " ");
    }
    printMiddle(song_win, 1,
                (COLS / 2) -
                    (strlen(getCurrTitle(song_menu->curitem->index)) / 2),
                strlen(getCurrTitle(song_menu->curitem->index)),
                getCurrTitle(song_menu->curitem->index), 1);
    refresh();
    break;
  case 'p':
    for (int i = 1; i < COLS - 1; i++) {
      mvprintw(1, i, " ");
    }
    printMiddle(song_win, 1,
                (COLS / 2) -
                    (strlen(getCurrTitle(song_menu->curitem->index)) / 2),
                strlen(getCurrTitle(song_menu->curitem->index)),
                getCurrTitle(song_menu->curitem->index), 1);
    loadSound(song_menu->curitem);
    int seconds = getSongDuration();
    mvwprintw(song_win, 1, COLS - 5, "%d:%d", (seconds/60)%60, seconds%60);
    refresh();
    break;
  }
  wrefresh(song_win);
}

void cleanupUI() { endwin(); }
