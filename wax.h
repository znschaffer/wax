#ifndef WAX_H
#define WAX_H

#include <taglib/tag_c.h> 
#include <ftw.h>
#include <menu.h>
#include <ncurses.h>
#include <stdio.h>

typedef struct song_file {
  char path[300];
  char artist[200];
  char title[200];
  char album[200];
  struct song_file* next;
} songs;

extern songs* songList;

songs* create_song(char *path);
songs* add_song(songs* songList, char *path);

/* Free all allocated memory and close ncurses */

void cleanupMA();
void cleanupUI();

void setupDir(char *dirPath);
/* Load song into engine and play it, stopping any currently playing song */
void loadSound(ITEM *item);

int getSongTime();

void toggleSong();

void restartSong();

/* Callback function for ftw() */
int addSongsToList(const char *path, const struct stat *sptr, int type);

/* Walks through dirPath, adding every .mp3 file to file_paths */
void loadDirectory(char *dirPath);

/* Loads every file in file_paths as an *ITEM in songs */
void loadSongs();

/* Basic ncurses setup code */
void setupCurses();

/* Initiliaze miniaudio engine */
int setupMA();

/* Switch case for handling user inputs */
void handleInput(int c);

/* Bootstraps ncurses menu with all the song names from loadSongs */
void setupUI();


/* Array of (ITEM *)songs */
extern ITEM **song_items;

/* Menu list holding songs */
extern MENU *song_menu;

/* Number of songs we know about */
extern int n_songs;

#endif // !WAX_HEADER
