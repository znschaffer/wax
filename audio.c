#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "wax.h"

ma_result result;
ma_engine engine;
ma_resource_manager rm;
ma_sound sound;
song *currSong;
bool soundInitialized;

int setupMA() {
  result = ma_engine_init(NULL, &engine);

  if (MA_SUCCESS != result) {
    cleanupMA();
    return -1;
  }
  rm = *ma_engine_get_resource_manager(&engine);
  return 0;
}

void loadSound(ITEM *item) {
  for (int i = 0; i < Library->num; i++) {
    song *s = Library->songs[i];
    if (0 == (strcmp(item->description.str, s->title)))
      if (0 == (strcmp(item->name.str, s->artist))) {
        currSong = s;
      }
  }

  if (NULL == currSong)
    return;

  // fprintf(log_file, "loadSound -- currSong->path = %s\n", currSong->path);
  ma_sound_uninit(&sound);
  ma_sound_init_from_file(&engine, currSong->path, 0, NULL, NULL, &sound);
  ma_sound_start(&sound);
  soundInitialized = true;
}

void toggleSong() {
  ma_sound_is_playing(&sound) ? ma_sound_stop(&sound) : ma_sound_start(&sound);
}

bool isPlaying() { return ma_sound_is_playing(&sound); }

void playNextSong() {
  if (NULL == currSong)
    return;
  currSong = Library->songs[(currSong->index + 1) % Library->num];
  ma_sound_uninit(&sound);
  ma_sound_init_from_file(&engine, currSong->path, 0, NULL, NULL, &sound);
  ma_sound_start(&sound);
}

bool checkIfSongFinished() {
  if (ma_sound_at_end(&sound)) {
    return true;
  }
  return false;
}

void playPrevSong() {
  if (NULL == currSong)
    return;
  if (currSong->index - 1 >= 0) {
    currSong = Library->songs[(currSong->index - 1) % Library->num];
  }
  ma_sound_uninit(&sound);
  ma_sound_init_from_file(&engine, currSong->path, 0, NULL, NULL, &sound);
  ma_sound_start(&sound);
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
  if (MA_SUCCESS != result) {
    return 0;
  }
  return (int)(pCursor);
}

void cleanupMA() {
  ma_sound_uninit(&sound);
  ma_engine_uninit(&engine);
}
