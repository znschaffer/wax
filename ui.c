#include "ncurses.h"
#include "wax.h"

ITEM **song_items;
ITEM **album_items;
ITEM **artist_items;

MENU *song_menu;
MENU *artist_menu;
MENU *album_menu;
WINDOW *window;
WINDOW *song_win;
char *title;
char *currArtist;
char *currAlbum;
int n_songs = 0;
int n_albums = 0;
int n_artists = 0;

void setupCurses() {
  initscr();
  cbreak();
  noecho();
  setupColors();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
}

void initWindow() {
  song_menu = new_menu((ITEM **)song_items);
  artist_menu = new_menu((ITEM **)artist_items);
  album_menu = new_menu((ITEM **)album_items);

  /* Init windows */
  window = newwin(0, 0, 0, 0);
  song_win = newwin(10, 10, COLS / 3, LINES - 10);
  keypad(song_win, TRUE);

  set_menu_win(song_menu, song_win);
  set_menu_sub(song_menu, derwin(song_win, LINES - 20, COLS - 20, 4, 2));
  set_menu_format(song_menu, 20, 1);
  set_menu_mark(song_menu, " > ");
}

void drawDefaultTitle() {
  printMiddle(window, 1, (COLS / 2) - (strlen(title) / 2), strlen(title), title,
              COLOR_PAIR(1));
}

void logSongItems(void) {
  for (int i = 0; i < n_songs; i++) {
    fprintf(log_file, "n_songs:%d:%s\n", i, song_items[i]->description.str);
  }
}

void logAlbumItems() {
  for (int i = 0; i < n_albums; i++) {
    fprintf(log_file, "n_albums:%d:%s\n", i, album_items[i]->name.str);
  }
}

void logArtistItems() {
  for (int i = 0; i < n_artists; i++) {
    fprintf(log_file, "n_artists:%d:%s\n", i, artist_items[i]->name.str);
  }
}

void logSongItems(void) {
  for (int i = 0; i < n_songs; i++) {
    fprintf(log_file, "n_songs:%d:%s\n", i, song_items[i]->description.str);
  }
}

void logAlbumItems() {
  for (int i = 0; i < n_albums; i++) {
    fprintf(log_file, "n_albums:%d:%s\n", i, album_items[i]->name.str);
  }
}

void logArtistItems() {
  for (int i = 0; i < n_artists; i++) {
    fprintf(log_file, "n_artists:%d:%s\n", i, artist_items[i]->name.str);
  }
}

void drawWindow() {
  box(window, 0, 0);
  box(song_win, 0, 0);
  title = "wax";
  drawDefaultTitle();
  mvwhline(window, 2, 1, ACS_HLINE, COLS - 2);
  mvwhline(window, LINES - 6, 1, ACS_HLINE, COLS - 2);
  printTime(window, LINES - 4, COLS - 8, convertToMins(SONG_DUR),
            COLOR_PAIR(2));

  refresh();
  post_menu(song_menu);
  wrefresh(song_win);
  wrefresh(window);
}

void printSongDuration() {
  printTime(window, LINES - 4, COLS - 8, convertToMins(SONG_DUR),
            COLOR_PAIR(2));
}

void printCurrTime() {
  printTime(window, LINES - 4, 2, convertToMins(SONG_CURRTIME), COLOR_PAIR(2));
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
  album_items = (ITEM **)calloc(400, sizeof(ITEM *));
  artist_items = (ITEM **)calloc(400, sizeof(ITEM *));

  populateArtistItems();
  // logArtistItems();
  populateAlbumItems(currArtist);
  // logAlbumItems();
  populateSongItems(currArtist, currAlbum);
  // logSongItems();

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
