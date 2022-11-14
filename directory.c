#include "wax.h"
#include <stdlib.h>

songs *songList;
TagLib_File *file;
TagLib_Tag *tag;

int parseDirectory(const char *path, const struct stat *sptr, int type) {
  if (type == FTW_F && strstr(path, ".mp3")) {
    songList = parseAudioFile(songList, (char *)path);
  }
  return 0;
}

void populateSongItems() {
  if (songList == NULL)
    return;
  for (songs *song = songList; song != NULL; song = song->next) {
    fprintf(log_file, "songList[%d] %s, %s, %s\n",n_songs, song->artist, song->title, song->path);
    ITEM *n_item = new_item(song->artist, song->title);
    if (n_item == NULL)
      continue;
    song_items[n_songs] = n_item;
    // memcpy(song_items[n_songs], &n_item, sizeof(ITEM));
    n_songs++;
  }
}

void logSongList(void) {
  songs *song;
  for (song = songList; NULL != song; song = song->next) {
    fprintf(log_file, "path: %s\n", song->path);
  }
}

int setupDir(char *dirPath) {
  ftw(dirPath, parseDirectory, 7);
  if (songList == NULL)
    return -1;
  return 0;
}

songs *newSongNode(char *path) {
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

songs *parseAudioFile(songs *songList, char *path) {
  songs *newSong = newSongNode(path);
  if (NULL != newSong) {
    newSong->next = songList;
  }
  return newSong;
}
