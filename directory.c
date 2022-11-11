#include "tag_c.h"
#include "wax.h"
#include <stdlib.h>
#include <string.h>

songs *songList;
FILE *log_file;
TagLib_File *file;
TagLib_Tag *tag;

int addSongsToList(const char *path, const struct stat *sptr, int type) {
  if (type == FTW_F && strstr(path, ".mp3")) {
    songList = add_song(songList, (char *)path);
    fprintf(log_file, "artist: %s \t path: %s\n", songList->artist,
            songList->path);
  }
  return 0;
}

void loadSongs() {
  int i = 0;
  for (songs *song = songList; song->next != NULL; song = song->next) {
    song_items[i] = new_item(song->artist, song->title);
    i++;
  }
}

void logSongList(void) {
  songs *song;
  for (song = songList; NULL != song; song = song->next) {
    fprintf(log_file, "path: %s", song->path);
  }
}

void setupDir(char *dirPath) {
  log_file = fopen("wax.log", "a+");
  loadDirectory(dirPath);
  mvprintw(LINES - 4, 0, "%s", songList->path);
  fclose(log_file);
}

void loadDirectory(char *dirPath) { ftw(dirPath, addSongsToList, 7); }

songs *create_song(char *path) {
  songs *newSong = malloc(sizeof(songs));
  if (NULL != newSong) {
    file = taglib_file_new(path);
    tag = taglib_file_tag(file);
    strcpy(newSong->artist, taglib_tag_artist(tag));
    strcpy(newSong->title, taglib_tag_title(tag));
    strcpy(newSong->album, taglib_tag_album(tag));
    strcpy(newSong->path, path);
    newSong->next = NULL;
    taglib_tag_free_strings();
    taglib_file_free(file);
  }
  return newSong;
}

songs *add_song(songs *songList, char *path) {
  songs *newSong = create_song(path);
  if (NULL != newSong) {
    newSong->next = songList;
  }
  return newSong;
}
