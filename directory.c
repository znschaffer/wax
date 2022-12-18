#include "wax.h"
#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

TagLib_File *file;
TagLib_Tag *tag;
int selection, maxx, maxy, len = 0, start = 0;
size_t total_files = 0;
library_t *Library;
directory_t *current_directory_ = NULL;

void get_files(int *len, char *basePath, char *target[]) {
  char path[1000];
  struct dirent *d_ent;
  DIR *dir = opendir(basePath);

  if (!dir)
    return;

  while ((d_ent = readdir(dir)) != NULL) {
    if (strcmp(d_ent->d_name, ".") != 0 && strcmp(d_ent->d_name, "..") != 0) {
      fprintf(log_file, "%s\n", d_ent->d_name);

      strcpy(path, basePath);
      strcat(path, "/");
      strcat(path, d_ent->d_name);

      get_files(len, path, target);
      target[*len] = strdup(path);
      *len += 1;
    }
  }

  closedir(dir);
}

void populateSongItems() {
  if (NULL == Library->songs[0])
    return;
  for (int i = 0; i < Library->num; i++) {
    ITEM *n_item =
        new_item(Library->songs[i]->artist, Library->songs[i]->title);
    if (NULL == n_item)
      continue;
    song_items[n_songs] = n_item;
    n_songs++;
  }
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

  current_directory_ = (directory_t *)malloc(sizeof(directory_t));

  if (current_directory_ == NULL) {
    printf("Error Occured.\n");
    exit(0);
  }

  getcwd(current_directory_->cwd, sizeof(current_directory_->cwd));
  strcat(current_directory_->cwd, "/");
  strcat(current_directory_->cwd, dirPath);

  char *files[200];
  get_files(&len, dirPath, files);
  Library->songs = calloc(len, sizeof(song *));

  for (int i = start; i < len; i++) {
    if (strstr(files[i], ".mp3") || strstr(files[i], ".wav")) {
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
      strcpy(Library->songs[currNum]->artist, artist);
      strcpy(Library->songs[currNum]->title, title);
      strcpy(Library->songs[currNum]->album, taglib_tag_album(tag));
      strcpy(Library->songs[currNum]->path, files[i]);
      Library->songs[currNum]->index = currNum;
      Library->num++;
    }
  }

  logLibrary();
  return 0;
}
