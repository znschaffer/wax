#include "tag_c.h"
#include "wax.h"
#include <dirent.h>
#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

TagLib_File *file;
TagLib_Tag *tag;
int selection, maxx, maxy, len = 0, start = 0;
size_t total_files = 0;
library_t *Library;
directory_t *current_directory_ = NULL;

void get_num_of_files(uint64_t *len, char *basePath) {
  char *path = calloc(100, sizeof(char));
  struct dirent *d_ent;
  DIR *dir = opendir(basePath);

  if (!dir) {
    free(path);
    return;
  }

  while ((d_ent = readdir(dir)) != NULL) {
    if (strcmp(d_ent->d_name, ".") != 0 && strcmp(d_ent->d_name, "..") != 0) {

      strcpy(path, basePath);
      strcat(path, "/");
      strcat(path, d_ent->d_name);

      get_num_of_files(len, path);

      if (strstr(d_ent->d_name, ".mp3") || strstr(d_ent->d_name, ".wav") ||
          strstr(d_ent->d_name, ".flac")) {
        *len += 1;
      }
    }
  }

  free(path);
  closedir(dir);
}

void get_files(int *len, char *basePath, char *target[]) {
  char *path = calloc(100, sizeof(char));
  struct dirent *d_ent;
  DIR *dir = opendir(basePath);

  if (!dir) {
    free(path);
    return;
  }

  while ((d_ent = readdir(dir)) != NULL) {
    if (strcmp(d_ent->d_name, ".") != 0 && strcmp(d_ent->d_name, "..") != 0) {

      strcpy(path, basePath);
      strcat(path, "/");
      strcat(path, d_ent->d_name);

      get_files(len, path, target);

      if (strstr(d_ent->d_name, ".mp3") || strstr(d_ent->d_name, ".wav") ||
          strstr(d_ent->d_name, ".flac")) {
        target[*len] = strdup(path);
        *len += 1;
      }
    }
  }

  free(path);
  closedir(dir);
}

void populateArtistItems() {

  if (NULL == Library->songs[0])
    return;

  char **ar = malloc(100 * sizeof(char *));
  n_artists = 0;
  int n_ar = 0;

  for (int i = 0; i < Library->num; i++) {
    bool unique = true;
    for (int j = 0; j < n_ar; j++) {
      if (strcmp(ar[j], Library->songs[i]->artist) == 0) {
        unique = false;
      }
    }
    if (unique) {
      ar[n_ar] = strdup(Library->songs[i]->artist);
      n_ar += 1;
    }
  }

  for (int i = 0; i < n_ar; i++) {
    ITEM *n_item = new_item(ar[i], "");
    if (NULL == n_item)
      continue;

    artist_items[n_artists] = n_item;
    n_artists++;
  }

  currArtist = (char *)artist_items[0]->name.str;
  free(ar);
}
void populateAlbumItems(char *artist) {
  if (NULL == Library->songs[0])
    return;

  char **al = malloc(100 * sizeof(char *));
  memset(album_items, 0, n_albums * sizeof(ITEM *));
  n_albums = 0;
  int n_al = 0;

  for (int i = 0; i < Library->num; i++) {
    if (strcmp(Library->songs[i]->artist, artist) == 0) {
      bool unique = true;
      for (int j = 0; j < n_al; j++) {
        if (strcmp(al[j], Library->songs[i]->album) == 0) {
          unique = false;
        }
      }
      if (unique) {
        al[n_al] = strdup(Library->songs[i]->album);
        n_al += 1;
      }
    }
  }
  for (int i = 0; i < n_al; i++) {
    ITEM *n_item = new_item(al[i], "");
    if (NULL == n_item)
      continue;

    album_items[n_albums] = n_item;
    n_albums++;
  }

  currAlbum = (char *)album_items[0]->name.str;
  free(al);
}

void swap(ITEM *xp, ITEM *yp) {
  ITEM temp = *xp;
  *xp = *yp;
  *yp = temp;
}

void sortSongItems() {
  int min_idx;
  for (int i = 0; i < n_songs - 1; i++) {
    min_idx = i;
    for (int j = i + 1; j < n_songs; j++) {
      if (atoi(song_items[j]->name.str) < atoi(song_items[min_idx]->name.str))
        min_idx = j;
    }

    swap(song_items[min_idx], song_items[i]);
  }
}

void populateSongItems(char *artist, char *album) {
  memset(song_items, 0, n_songs * sizeof(ITEM *));
  n_songs = 0;
  if (NULL == Library->songs[0])
    return;
  for (int i = 0; i < Library->num; i++) {
    if ((strcmp(Library->songs[i]->album, album) == 0) &&
        (strcmp(Library->songs[i]->artist, artist) == 0)) {

      char *trackNum = calloc(0, 4 * sizeof(char));
      sprintf(trackNum, "%d", Library->songs[i]->track_n);
      ITEM *n_item = new_item(trackNum, Library->songs[i]->title);
      if (NULL == n_item)
        continue;
      song_items[n_songs] = n_item;
      n_songs++;
    }
  }
  sortSongItems();
}

void logLibrary(void) {
  for (int i = 0; i < Library->num; i++) {
    fprintf(log_file, "Library->songs[%d]->path = %s\n", i,
            Library->songs[i]->path);
  }
}

int setupDir(char *dirPath) {
  Library = calloc(0, sizeof(*Library));
  Library->num = 0;

  uint64_t file_num = 0;
  get_num_of_files(&file_num, dirPath);
  char *files[file_num];
  get_files(&len, dirPath, files);
  Library->songs = calloc(len, sizeof(song *));

  for (int i = start; i < len; i++) {
    if (strstr(files[i], ".mp3") || strstr(files[i], ".wav") ||
        strstr(files[i], ".flac")) {
      int currNum = Library->num;
      Library->songs[currNum] = calloc(1, sizeof(song));
      file = taglib_file_new(files[i]);
      tag = taglib_file_tag(file);
      char *title = taglib_tag_title(tag);
      if (strcmp(title, "") == 0) {
        title = basename(files[i]);
      }
      char *artist = taglib_tag_artist(tag);
      if (strcmp(artist, "") == 0) {
        artist = "--";
      }
      int trackNum = taglib_tag_track(tag);
      Library->songs[currNum]->track_n = trackNum;
      strcpy(Library->songs[currNum]->artist, artist);
      strcpy(Library->songs[currNum]->title, title);
      strcpy(Library->songs[currNum]->album, taglib_tag_album(tag));
      strcpy(Library->songs[currNum]->path, files[i]);
      Library->songs[currNum]->index = currNum;
      Library->num++;
      taglib_tag_free_strings();
      taglib_file_free(file);
      free(files[i]);
    }
  }

  logLibrary();
  return 0;
}
