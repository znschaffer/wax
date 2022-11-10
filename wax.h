#ifndef WAX_HEADER
#define WAX_HEADER

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <ncurses.h>
void setupCurses();
void mainLoop();
void handleInput(int c);

extern ma_result result;
extern ma_engine engine;
extern ma_sound sound;

#endif // !WAX_HEADER
