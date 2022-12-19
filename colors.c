#include "wax.h"

void setupColors() {
  start_color();

  init_pair(0, COLOR_BLACK, COLOR_WHITE);
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLACK);
}
