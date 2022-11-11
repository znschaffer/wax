#include "wax.h"
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: wax <path_to_music_dir>.\n");
    return -1;
  }

  setupDir(argv[1]);
  setupMA();
  setupUI();

  cleanupMA();
  cleanupUI();
}
