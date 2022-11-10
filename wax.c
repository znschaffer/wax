#include "wax.h"
#include <stdio.h>

ma_result result;
ma_engine engine;
ma_sound sound;

int main(int argc, char **argv) {
  setupCurses();

  if (argc < 2) {
    printf("No input file.");
    return -1;
  }

  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    printf("Failed to init audio engine.");
    return -1;
  }

  result = ma_sound_init_from_file(&engine, argv[1], 0, NULL, NULL, &sound);
  printw("loaded song from %s\n", argv[1]);
  mainLoop();

  ma_sound_uninit(&sound);
  ma_engine_uninit(&engine);
  endwin();
}

void handleInput(int c) {
  if (ma_sound_is_playing(&sound)) {
    ma_sound_stop(&sound);
  } else {
    ma_sound_start(&sound);
  }
}

void setupCurses() {
  initscr();
  noecho();
  curs_set(0);
}

void mainLoop() {
  int c;

  printw("p to toggle, q to quit\n");
  while ((c = getch())) {
    if (c == 'q') {
      break;
    }

    if (c == 'p') {
      handleInput(c);
    }
  }
}
