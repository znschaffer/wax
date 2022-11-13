#include "wax.h"

int n_songs = 0;
int main(int argc, char **argv) {
  int ret;
  if (argc < 2) {
    printf("usage: wax <path_to_music_dir>.\n");
    return -1;
  }

  ret = setupDir(argv[1]);
  if (ret != 0) {
    fprintf(stderr, "failed to load directory %s", argv[1]);
    return -1;
  }
  ret = setupMA();
  if (ret != 0) {
    fprintf(stderr, "failed to load audio engine");
    return -1;
  }

  ret = setupUI();
  if (ret != 0) {
    fprintf(stderr, "failed to load ncurses UI");
    return -1;
  }

  cleanupMA();
  cleanupUI();
}
