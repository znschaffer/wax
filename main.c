#include "wax.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: wax <path_to_music_dir>.\n");
    cleanup();
    return -1;
  }

  loadDirectory(argv[1]);

  setupMA();
  setupUI();

  cleanup();
}
