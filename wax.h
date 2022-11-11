#ifndef WAX_H
#define WAX_H

#include <ftw.h>
#include <ncurses/menu.h>
#include <ncurses/ncurses.h>
#include <stdio.h>

/* Free all allocated memory and close ncurses */
void cleanup();

/* Load song into engine and play it, stopping any currently playing song */
void loadSound(char *songpath);

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

/* Array of music files in directory */
extern char file_paths[200][100];

/* Array of (ITEM *)songs */
extern ITEM **songs;

/* Menu list holding songs */
extern MENU *song_menu;

/* Number of songs we know about */
extern int n_songs;

#endif // !WAX_HEADER
