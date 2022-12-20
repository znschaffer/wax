#include "wax.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>
FILE *log_file;

CONFIG *config;

int parse_config(char *buf) {
  char dummy[256];
  char unformatted_dir[256];
  if (sscanf(buf, " %s", dummy) == EOF)
    return 0; // blank line
  if (sscanf(buf, " %[#]", dummy) == 1)
    return 0; // comment
  if (sscanf(buf, " music_dir = %s", unformatted_dir) == 1) {
    wordexp_t exp_result;
    wordexp(unformatted_dir, &exp_result, 0);
    strcpy(config->music_dir, exp_result.we_wordv[0]);
    wordfree(&exp_result);
    return 0;
  }
  return 3;
}

int main(int argc, char **argv) {
  config = calloc(0, sizeof(CONFIG));
  FILE *c_f;
  bool dirflag;
  const char *home = getenv("HOME");
  char *configpath = calloc(0, 250 * sizeof(char));
  strlcpy(configpath, home, 250);
  strlcat(configpath, "/.config/wax/config", 250);
  if (access(configpath, F_OK) == 0) {
    c_f = fopen(configpath, "r");
  } else {
    c_f = fopen(configpath, "w");
    fprintf(c_f, "music_dir = %s/Music\n", home);
  }

  int ret;
  int opt;
  while ((opt = getopt(argc, argv, "cdh")) != -1) {
    switch (opt) {
    case 'd':
      dirflag = true;
      break;
    default:
      fprintf(stderr, "Usage: %s [-d music_dir] \n", argv[0]);
      exit(1);
    }
  }

  char buf[256];
  while (fgets(buf, sizeof buf, c_f)) {
    parse_config(buf);
  }
  log_file = fopen("wax.log", "a+");
  fprintf(log_file, "config: %s\n", config->music_dir);

  if (dirflag) {
    strcpy(config->music_dir, argv[optind]);
  }
  if ((strcmp(config->music_dir, "") == 0)) {
    fprintf(stderr, "Music directory not found. Either specify with `-d "
                    "dirname` or in your config file.\n");
    return -1;
  }
  ret = setupDir(config->music_dir);
  if (0 != ret) {
    // fprintf(stderr, "failed to load directory %s", argv[1]);
    fclose(log_file);
    return -1;
  }
  ret = setupMA();
  if (0 != ret) {
    // fprintf(stderr, "failed to load audio engine");
    fclose(log_file);
    return -1;
  }

  ret = setupUI();
  if (0 != ret) {
    // fprintf(stderr, "failed to load ncurses UI");
    fclose(log_file);
    return -1;
  }

  cleanupMA();
  cleanupUI();
  fclose(log_file);
}
