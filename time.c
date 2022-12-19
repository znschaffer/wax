#include "wax.h"

Time convertToMins(int sec) {
  int h, m, s;

  char timestring[20];

  Time convertedTime;

  convertedTime.hours = h = (sec / 3600);

  convertedTime.min = m = (sec - (3600 * h)) / 60;

  convertedTime.sec = s = (sec - (3600 * h) - (m * 60));

  if (h > 0) {
    sprintf(timestring, "%d:%s%d:%s%d", h, m < 10 ? "0" : "", m,
            s < 10 ? "0" : "", s);
  } else {
    sprintf(timestring, "%s%d:%s%d", m < 10 ? "0" : "", m, s < 10 ? "0" : "",
            s);
  }

  strcpy(convertedTime.timestring, timestring);

  return convertedTime;
}

void printTime(WINDOW *window, int rows, int cols, Time time, chtype color) {
  wattron(window, color);
  mvwprintw(window, rows, cols, "%s", time.timestring);
  wattroff(window, color);

  refresh();
}
