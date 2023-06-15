#ifndef MAIN_H
#define MAIN_H
#include "common.h"
Ant *ant;
Food *food;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool started = false;
const int SCREEN_WIDTH = 1920 / 2;
const int SCREEN_HEIGHT = 1080 / 2;
const int ANT_SIZE = 10;
const int FOOD_SIZE = 30;
const float bounce[2] = {PI - PI / 4, PI + PI / 4};

void readConstants(char *file_name);

void *updateAnt_thread(void *args);
#endif