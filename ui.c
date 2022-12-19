#include "ncurses.h"
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
  setupColors();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
}

void logSongItems(void) {
  for (int i = 0; i < n_songs; i++) {
    fprintf(log_file, "n_songs:%d:%d\n", i, song_items[i]->index);
  }
}

void initWindow() {
  song_menu = new_menu((ITEM **)song_items);

  /* Init windows */
  song_win = newwin(0, 0, 0, 0);
  keypad(song_win, TRUE);

  set_menu_win(song_menu, song_win);
  set_menu_sub(song_menu, derwin(song_win, LINES - 10, COLS - 10, 4, 2));
  set_menu_format(song_menu, LINES - 15, 1);
  set_menu_mark(song_menu, " > ");
}

void drawDefaultTitle() {
  printMiddle(song_win, 1, (COLS / 2) - (strlen(title) / 2), strlen(title),
              title, COLOR_PAIR(1));
}

void drawWindow() {
  box(song_win, 0, 0);
  title = "wax";
  drawDefaultTitle();
  mvwhline(song_win, 2, 1, ACS_HLINE, COLS - 2);
  mvwhline(song_win, LINES - 6, 1, ACS_HLINE, COLS - 2);
  printTime(song_win, LINES - 4, COLS - 8, convertToMins(SONG_DUR),
            COLOR_PAIR(2));

  refresh();
  post_menu(song_menu);
  wrefresh(song_win);
}

void printSongDuration() {
  printTime(song_win, LINES - 4, COLS - 8, convertToMins(SONG_DUR),
            COLOR_PAIR(2));
}

void printCurrTime() {
  printTime(song_win, LINES - 4, 2, convertToMins(SONG_CURRTIME),
            COLOR_PAIR(2));
}

/* Refreshing Title*/
void redrawBlank() {
  for (int i = 1; i < COLS - 1; i++) {
    mvwprintw(song_win, 1, i, " ");
  }
}

void redrawTitle() {
  redrawBlank();
  printMiddle(song_win, 1,
              (COLS / 2) -
                  (strlen(getCurrTitle(song_menu->curitem->index)) / 2),
              strlen(getCurrTitle(song_menu->curitem->index)),
              getCurrTitle(song_menu->curitem->index), COLOR_PAIR(1));
  printSongDuration();
  refresh();
}

void setSongTime() { SONG_CURRTIME = getSongTime(); }
void setSongDur() { SONG_DUR = getSongDuration(); }
int setupUI() {
  int ch;
  setupCurses();
  setupColors();
  song_items = (ITEM **)calloc(400, sizeof(ITEM *));
  populateSongItems();

  /* Set up Window functionality*/
  initWindow();

  /* Draw Window UI*/
  drawWindow();
  while ('q' != (ch = getch())) {
    setSongTime();
    printCurrTime();

    drawTicker(song_win);
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
  case KEY_RIGHT:
    playNextSong();
    menu_driver(song_menu, REQ_DOWN_ITEM);
    setSongDur();
    redrawTitle();
    break;
  case '<':
  case KEY_LEFT:
    playPrevSong();
    menu_driver(song_menu, REQ_UP_ITEM);
    setSongDur();
    redrawTitle();
    break;
  case ' ':
    toggleSong();
    redrawTitle();
    refresh();
    break;
  case 'p':
    loadSound(song_menu->curitem);
    setSongDur();
    redrawTitle();
    refresh();
    break;
  }
  wrefresh(song_win);
}

void cleanupUI() { endwin(); }
