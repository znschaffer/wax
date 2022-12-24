#include "curses.h"
#include "wax.h"

void setupColors() {
  start_color();
  assume_default_colors(config->fg, config->bg);
  // main fg + bg
  init_pair(1, config->fg, config->bg);

  // main highlight
  init_pair(2, config->hl, config->bg);

  // reverse highlight
  init_pair(3, config->bg, config->hl);

}
