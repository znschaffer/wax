#include "wax.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>
FILE *log_file;

CONFIG *config;

int main(int argc, char **argv) {
  config = &(CONFIG){.bg = COLOR_BLACK, .fg = COLOR_WHITE, .hl = COLOR_CYAN};
  bool dirflag;
  const char *home = getenv("HOME");
  char *configfolder;
  char *configfile;
  char *logfilepath = calloc(0, 250 * sizeof(char));

  asprintf(&configfolder, "%s/.config/wax", home);
  asprintf(&configfile, "%s/config", configfolder);

  mkdir(configfolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  if (access(configfile, F_OK) != 0) {
    FILE *newConfig = fopen(configfile, "w");
    fprintf(newConfig,
            "# Path to your music\n"
            "music_dir = %s/Music\n\n"
            "# Background ANSI color\n"
            "bg = 7\n\n"
            "# Foreground ANSI color\n"
            "fg = 0\n\n"
            "# Highlight ANSI color\n"
            "hl = 4",
            home);
    fclose(newConfig);
  }

  FILE *c_f = fopen(configfile, "r");
  free(configfile);
  free(configfolder);

  strlcpy(logfilepath, home, 250);
  strlcat(logfilepath, "/.config/wax", 250);
  strlcat(logfilepath, "/wax.log", 250);

  log_file = fopen(logfilepath, "a+");
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

  fclose(c_f);
  fprintf(log_file, "config: %s\n", config->music_dir);

  if (dirflag) {
    strcpy(config->music_dir, argv[optind]);
  }
  if ((strcmp(config->music_dir, "") == 0)) {
    fprintf(stderr, "Music directory not found. Either specify with `-d "
                    "dirname` or in your config file.\n");
    return -1;
  }
  int ret = setupDir(config->music_dir);
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
