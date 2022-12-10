#include "wax.h"
#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>

songs *songList;
TagLib_File *file;
TagLib_Tag *tag;
int selection, maxx, maxy, len = 0, start = 0;
size_t total_files = 0;
library_t *Library;
directory_t *current_directory_ = NULL;
int get_files(char *directory, char *target[]) {
  int i = 0;
  DIR *dir_;
  struct dirent *dir_entry;

  dir_ = opendir(directory);
  if (NULL == dir_) {
    return -1;
  }

  while ((dir_entry = readdir(dir_)) != NULL) {
    if (strcmp(dir_entry->d_name, ".") != 0) {
      target[i++] = strdup(dir_entry->d_name);
    }
  }
  closedir(dir_);
  return 1;
}

void populateSongItems() {
  if (NULL == Library)
    return;
  for (int i = 0; i < Library->num; i++) {
      fprintf(log_file, "%s\n", Library->songs[i]->path);
    ITEM *n_item =
        new_item(Library->songs[i]->artist, Library->songs[i]->title);
    if (NULL == n_item)
      continue;
    song_items[n_songs] = n_item;
    n_songs++;
  }
}

void logSongList(void) {
  songs *song;
  for (song = songList; NULL != song; song = song->next) {
    fprintf(log_file, "path: %s\n", song->path);
  }
}

int get_no_files_in_directory(char *directory) {
  int len = 0;
  DIR *dir_;
  struct dirent *dir_entry;

  dir_ = opendir(directory);
  if (dir_ == NULL) {
    return -1;
  }

  while ((dir_entry = readdir(dir_)) != NULL) {
    // Skip .
    if (strcmp(dir_entry->d_name, ".") != 0) {
      len++;
    }
  }
  closedir(dir_);
  return len;
}

char *get_parent_directory(char *cwd) {
  char *a;
  a = strdup(cwd);
  int i = strlen(a) - 1;
  while (a[--i] != '/')
    ;
  a[++i] = '\0';
  return a;
}

int setupDir(char *dirPath) {
  songList = NULL;
  Library = calloc(0, sizeof(*Library));
  Library->num = 0;

  current_directory_ = (directory_t *)malloc(sizeof(directory_t));
  if (current_directory_ == NULL) {
    printf("Error Occured.\n");
    exit(0);
  }
  getcwd(current_directory_->cwd, sizeof(current_directory_->cwd));
  strcat(current_directory_->cwd, "/");
  current_directory_->parent_dir =
      strdup(get_parent_directory(current_directory_->cwd));
  len = get_no_files_in_directory(dirPath);
  char *files[10];
  get_files(dirPath, files);
    fprintf(log_file, "%s\n", files[0]);

  for (int i = start; i < len; i++) {
    fprintf(log_file, "%s\n", files[i]);
    if (strstr(files[i], ".mp3")) {
      Library->num++;
      strcpy(Library->songs[i]->album, "hi");
      strcpy(Library->songs[i]->artist, "hi");
      strcpy(Library->songs[i]->path, "hi");
      strcpy(Library->songs[i]->title, "hi");
      // insertSongNode(&songList, files[i]);
    }
  }
  return 0;
}

void insertSongNode(songs **songList, char *path) {
  songs *newNode = malloc(sizeof(songs));
  if (NULL != newNode) {
    file = taglib_file_new(path);
    tag = taglib_file_tag(file);
    char *title = taglib_tag_title(tag);
    if (strcmp(title, "") == 0) {
      title = basename(path);
    }
    char *artist = taglib_tag_artist(tag);
    if (strcmp(artist, "") == 0) {
      artist = "--";
    }

    fprintf(log_file, "title: %s, artist: %s\n", title, artist);

    strcpy(newNode->artist, artist);
    strcpy(newNode->title, title);
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
