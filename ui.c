#include "menu.h"
#include "ncurses.h"
#include "wax.h"

#define HEIGHT_CUTOFF 12

#define TIME_HEIGHT (LINES - 4)
#define TICKER_HEIGHT (LINES - 4)
#define TITLE_HEIGHT (1)

#define TIME_HEIGHT_SHORT (LINES - 4)
#define TICKER_HEIGHT_SHORT (LINES - 4)
#define TITLE_HEIGHT_SHORT (4)

#define MENU_LENGTH (LINES - 10)
#define MENU_FORMAT_LENGTH (LINES - 11)
#define MENU_WIDTH ((COLS / 4) - 2)
#define SONG_MENU_WIDTH ((COLS / 2) - 3)

#define WINDOW_LENGTH (LINES - 9)
#define WINDOW_WIDTH ((COLS / 4) - 1)
#define SONG_WINDOW_WIDTH ((COLS / 2) - 2)

ITEM **song_items;
ITEM **album_items;
ITEM **artist_items;

MENU *song_menu;
MENU *artist_menu;
MENU *album_menu;
MENU *curr_menu;
WINDOW *window;
WINDOW *song_win;
WINDOW *artist_win;
WINDOW *album_win;
WINDOW *curr_win;

char *title;
char *currArtist;
char *currAlbum;
int n_songs = 0;
int n_albums = 0;
int n_artists = 0;

void redrawFocus() {
  // Reset Colors
  set_menu_fore(song_menu, A_NORMAL);
  set_menu_fore(album_menu, A_NORMAL);
  set_menu_fore(artist_menu, A_NORMAL);
  set_menu_back(song_menu, A_DIM);
  set_menu_back(album_menu, A_DIM);
  set_menu_back(artist_menu, A_DIM);

  // Set Focused Colors
  set_menu_fore(curr_menu, A_STANDOUT);
  set_menu_back(curr_menu, A_NORMAL);
}

// Logging
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

// Init CURSES
void setupCurses() {
  initscr();
  cbreak();
  noecho();
  setupColors();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
}

void attachMenu(MENU *menu, WINDOW *win) {
  set_menu_win(menu, win);
  if (menu == song_menu) {
    set_menu_sub(menu, derwin(win, MENU_LENGTH, SONG_MENU_WIDTH, 1, 1));
  } else {
    set_menu_sub(menu, derwin(win, MENU_LENGTH, MENU_WIDTH, 1, 1));
  }
  set_menu_format(menu, MENU_FORMAT_LENGTH, 0);
  set_menu_mark(menu, "* ");
}

void initMenus() {
  song_menu = new_menu((ITEM **)song_items);
  artist_menu = new_menu((ITEM **)artist_items);
  album_menu = new_menu((ITEM **)album_items);
}

// Setup all windows and menus
void setupWindows() {
  window = newwin(0, 0, 0, 0);
  artist_win = newwin(WINDOW_LENGTH, WINDOW_WIDTH, 3, 1);                 // 1
  album_win = newwin(WINDOW_LENGTH, WINDOW_WIDTH, 3, (COLS / 4) + 1);     // 2
  song_win = newwin(WINDOW_LENGTH, SONG_WINDOW_WIDTH, 3, (COLS / 2) + 1); // 3

  initMenus();
  attachMenu(artist_menu, artist_win);
  attachMenu(album_menu, album_win);
  attachMenu(song_menu, song_win);

  keypad(song_win, TRUE);
}

void drawDefaultTitle() {
  if (LINES < HEIGHT_CUTOFF) {
    printMiddle(window, TITLE_HEIGHT_SHORT, (COLS / 2) - (strlen(title) / 2),
                strlen(title), title, COLOR_PAIR(1));
  } else {
    printMiddle(window, TITLE_HEIGHT, (COLS / 2) - (strlen(title) / 2),
                strlen(title), title, COLOR_PAIR(1));
  }
}

// Basic UI and outlines for outer window
void drawWindow() {
  box(window, 0, 0);
  title = "wax";
  drawDefaultTitle();
  if (LINES < HEIGHT_CUTOFF) {

    printTime(window, TIME_HEIGHT_SHORT, COLS - 8, convertToMins(SONG_DUR),
              COLOR_PAIR(1));
  } else {

    printTime(window, TIME_HEIGHT, COLS - 8, convertToMins(SONG_DUR),
              COLOR_PAIR(1));
  }
  refresh();
  drawHotkeyBar();

  if (LINES < HEIGHT_CUTOFF) {
  } else {
    mvwhline(window, 2, 1, ACS_HLINE, COLS - 2);
    mvwhline(window, LINES - 6, 1, ACS_HLINE, COLS - 2);
    mvwvline(window, 3, (COLS / 4), ACS_VLINE, LINES - 9);
    mvwvline(window, 3, (COLS / 2), ACS_VLINE, LINES - 9);
  }
  wrefresh(window);
}

// Basic UI for menus
void drawMenus() {
  if (LINES < HEIGHT_CUTOFF) {
    wrefresh(artist_win);
    wrefresh(album_win);
    wrefresh(song_win);
  } else {
    post_menu(artist_menu);
    post_menu(album_menu);
    post_menu(song_menu);

    wrefresh(artist_win);
    wrefresh(album_win);
    wrefresh(song_win);
  }
}

void printSongDuration() {
  if (LINES < HEIGHT_CUTOFF) {
    printTime(NULL, TIME_HEIGHT_SHORT, COLS - 8, convertToMins(SONG_DUR),
              COLOR_PAIR(1));
  } else {

    printTime(NULL, TIME_HEIGHT, COLS - 8, convertToMins(SONG_DUR),
              COLOR_PAIR(1));
  }
}

void printCurrTime() {

  if (LINES < HEIGHT_CUTOFF) {
    printTime(NULL, TIME_HEIGHT_SHORT, 2, convertToMins(SONG_CURRTIME),
              COLOR_PAIR(1));
  } else {
    printTime(NULL, TIME_HEIGHT, 2, convertToMins(SONG_CURRTIME),
              COLOR_PAIR(1));
  }
}

/* Refreshing Title*/
void redrawBlank() {
  if (LINES < HEIGHT_CUTOFF) {
    for (int i = 1; i < COLS - 2; i++) {
      mvprintw(TITLE_HEIGHT_SHORT, i, " ");
    }
  } else {
    for (int i = 1; i < COLS - 2; i++) {
      mvprintw(TITLE_HEIGHT, i, " ");
    }
  }
}

void redrawTitle() {
  redrawBlank();
  if (LINES < HEIGHT_CUTOFF) {
    printMiddle(NULL, TITLE_HEIGHT_SHORT,
                (COLS / 2) -
                    (strlen(getCurrTitle(song_menu->curitem->index)) / 2),
                strlen(getCurrTitle(song_menu->curitem->index)),
                getCurrTitle(song_menu->curitem->index), COLOR_PAIR(1));
  } else {
    printMiddle(NULL, TITLE_HEIGHT,
                (COLS / 2) -
                    (strlen(getCurrTitle(song_menu->curitem->index)) / 2),
                strlen(getCurrTitle(song_menu->curitem->index)),
                getCurrTitle(song_menu->curitem->index), COLOR_PAIR(1));
  }
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
  populateAlbumItems(currArtist);
  populateSongItems(currArtist, currAlbum);

  /* Set up Window functionality*/
  setupWindows();

  /* Draw Window UI*/
  drawWindow();

  /* Draw Menu UI */
  drawMenus();

  curr_menu = song_menu;
  curr_win = song_win;

  while ('q' != (ch = getch())) {
    setSongTime();
    printCurrTime();
    drawTicker();
    handleInput(ch);
  }

  cleanupUI();
  return 0;
}

void logSongMenu() {
  for (int i = 0; i < n_songs; i++) {
    fprintf(log_file, "song_menu[%d] = %s\n", i, song_menu->items[i]->name.str);
  }
}

void moveFocusLeft() {
  if (curr_menu == artist_menu) {
    return;
  } else if (curr_menu == album_menu) {
    curr_menu = artist_menu;
    curr_win = artist_win;
  } else if (curr_menu == song_menu) {
    curr_menu = album_menu;
    curr_win = album_win;
  }
}
void moveFocusRight() {
  if (curr_menu == song_menu) {
    return;
  } else if (curr_menu == album_menu) {
    curr_menu = song_menu;
    curr_win = song_win;
  } else if (curr_menu == artist_menu) {
    curr_menu = album_menu;
    curr_win = album_win;
  }
}

void handleInput(int ch) {
  switch (ch) {
  case KEY_RESIZE:
    clear();
    endwin();

    initMenus();
    /* Set up Window functionality*/
    setupWindows();
    /* Draw Window UI*/
    drawWindow();
    /* Draw Menu UI */
    drawMenus();

    drawTicker();
    redrawTitle();
    curr_menu = song_menu;
    curr_win = song_win;
    refresh();
    break;
  case 'h':
    moveFocusLeft();
    break;
  case 'l':
    moveFocusRight();
    break;
  case 'j':
  case KEY_DOWN:
    menu_driver(curr_menu, REQ_DOWN_ITEM);
    break;
  case 'k':
  case KEY_UP:
    menu_driver(curr_menu, REQ_UP_ITEM);
    break;
  case ']':
    skipAhead();
    break;
  case '[':
    skipBack();
    break;
  case KEY_LEFT:
    moveFocusLeft();
    break;
  case KEY_RIGHT:
    moveFocusRight();
    break;
  case '.':
    playNextSong();
    menu_driver(curr_menu, REQ_DOWN_ITEM);
    setSongDur();
    redrawTitle();
    break;
  case ',':
    playPrevSong();
    menu_driver(curr_menu, REQ_UP_ITEM);
    setSongDur();
    redrawTitle();
    break;
  case 'p':
    if (curr_menu == song_menu) {
      loadSound(curr_menu->curitem);
      setSongDur();
      redrawTitle();
    } else if (curr_menu == album_menu) {
      currAlbum = (char *)curr_menu->curitem->name.str;
      populateSongItems(currArtist, currAlbum);
      unpost_menu(song_menu);
      set_menu_items(song_menu, song_items);
      drawMenus();
    } else if (curr_menu == artist_menu) {
      currArtist = (char *)curr_menu->curitem->name.str;
      populateAlbumItems(currArtist);
      populateSongItems(currArtist, currAlbum);

      unpost_menu(album_menu);
      set_menu_items(album_menu, album_items);
      unpost_menu(song_menu);
      set_menu_items(song_menu, song_items);
      drawMenus();
    }

    refresh();
    break;
  }
  redrawFocus();
  wrefresh(song_win);
  wrefresh(album_win);
  wrefresh(artist_win);
}

void cleanupUI() { endwin(); }
