#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "wax.h"

ma_result result;
ma_engine engine;
ma_resource_manager rm;
ma_sound sound;

int setupMA() {
  result = ma_engine_init(NULL, &engine);

  if (result != MA_SUCCESS) {
    cleanupMA();
    return -1;
  }
  rm = *ma_engine_get_resource_manager(&engine);
  return 0;
}

void loadSound(ITEM *item) {
  songs *song;
  char *path;
  for (song = songList; NULL != song; song = song->next) {
    if ((strcmp(item->description.str, song->title) == 0))
      if ((strcmp(item->name.str, song->artist) == 0))
        path = song->path;
  }

  if (path == NULL)
    return;

  ma_sound_uninit(&sound);
  ma_sound_init_from_file(&engine, path, 0, NULL, NULL, &sound);
  ma_sound_start(&sound);
}

void toggleSong() {
  ma_sound_is_playing(&sound) ? ma_sound_stop(&sound) : ma_sound_start(&sound);
}

bool isPlaying() {
  return ma_sound_is_playing(&sound);
}

int getSongDuration() {
  float length;
  ma_sound_get_length_in_seconds(&sound, &length);
  return (int)length;
}

void restartSong() { ma_sound_seek_to_pcm_frame(&sound, 0); }

int getSongTime() {
  float pCursor;
  result = ma_sound_get_cursor_in_seconds(&sound, &pCursor);
  if (result != MA_SUCCESS) {
    return 0;
  }
  return (int)(pCursor);
}

void cleanupMA() {
  ma_sound_uninit(&sound);
  ma_engine_uninit(&engine);
}
