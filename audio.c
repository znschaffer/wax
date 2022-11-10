#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "wax.h"

ma_result result;
ma_engine engine;
ma_sound sound;

int setupMA() {
  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    printf("Failed to init audio engine.\n");
    cleanup();
    return -1;
  }
  return 0;
}

void loadSound(char *songpath) {
  if (ma_sound_is_playing(&sound)) {
    ma_sound_stop(&sound);
    ma_sound_uninit(&sound);
  }
  ma_sound_init_from_file(&engine, songpath, 0, NULL, NULL, &sound);
  ma_sound_start(&sound);
}

int addSongsToList(const char *path, const struct stat *sptr, int type) {
  if (type == FTW_F && strstr(path, ".mp3")) {
    mvprintw(LINES - 3, 0, "%s", (char *)path);
    refresh();
    strcpy(file_paths[n_songs], path);
    n_songs++;
  }
  return 0;
}

void loadSongs() {
  for (int i = 0; i < n_songs; i++) {
    songs[i] = new_item(file_paths[i], file_paths[i]);
  }
}

void cleanup() {

  ma_sound_uninit(&sound);
  ma_engine_uninit(&engine);
  endwin();
}
