#include "wax.h"

songs *songList;
TagLib_File *file;
TagLib_Tag *tag;

int parseDirectory(const char *path, const struct stat *sptr, int type) {
  if (FTW_F == type && strstr(path, ".mp3")) {
    insertSongNode(&songList, (char *)path);
  }
  return 0;
}

void populateSongItems() {
  if (NULL == songList)
    return;
  for (songs *song = songList; NULL != song; song = song->next) {
    fprintf(log_file, "songList[%d] %s, %s, %s\n", n_songs, song->artist,
            song->title, song->path);
    ITEM *n_item = new_item(song->artist, song->title);
    if (NULL == n_item)
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
  songList = NULL;
  ftw(dirPath, parseDirectory, 7);
  if (NULL == songList) {
    return -1;
  }
  return 0;
}

void insertSongNode(songs **songList, char *path) {
  songs *newNode = malloc(sizeof(songs));
  if (NULL != newNode) {
    file = taglib_file_new(path);
    tag = taglib_file_tag(file);
    strcpy(newNode->artist, taglib_tag_artist(tag));
    strcpy(newNode->title, taglib_tag_title(tag));
    strcpy(newNode->album, taglib_tag_album(tag));
    strcpy(newNode->path, path);
    newNode->next = NULL;
    if (NULL == *songList) {
      *songList = newNode;
      newNode->prev = NULL;
      return;
    }

    songs *temp = *songList;

    while (NULL != temp->next)
      temp = temp->next;

    temp->next = newNode;
    newNode->prev = temp;
    taglib_tag_free_strings();
    taglib_file_free(file);
  }
}
