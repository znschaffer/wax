#include "wax.h"
FILE *log_file;
int main(int argc, char **argv) {
  int ret;
  if (argc < 2) {
    printf("usage: wax <path_to_music_dir>.\n");
    return -1;
  }
  log_file = fopen("wax.log", "a+");
  ret = setupDir(argv[1]);
  if (ret != 0) {
    fprintf(stderr, "failed to load directory %s", argv[1]);
    fclose(log_file);
    return -1;
  }
  ret = setupMA();
  if (ret != 0) {
    fprintf(stderr, "failed to load audio engine");
    fclose(log_file);
    return -1;
  }

  ret = setupUI();
  if (ret != 0) {
    fprintf(stderr, "failed to load ncurses UI");
    fclose(log_file);
    return -1;
  }

  cleanupMA();
  cleanupUI();
  fclose(log_file);
}
