#include "ncurses.h"
#include "wax.h"

#include <wordexp.h>

int parse_config(char *buf) {
    
  char dummy[256];
  char unformatted_dir[256];
  int temp_color;
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
  if (sscanf(buf, " bg = %d", &temp_color) != 0) {
      if (temp_color >= 0 && temp_color <= 15) {
          config->bg = temp_color;
      }
  }
  if (sscanf(buf, " fg = %d", &temp_color) != 0) {
      if (temp_color >= 0 && temp_color <= 15) {
          config->fg = temp_color;
      }
  }
  if (sscanf(buf, " hl = %d", &temp_color) != 0) {
      if (temp_color >= 0 && temp_color <= 15) {
          config->hl = temp_color;
      }
  }

  return 3;
}
