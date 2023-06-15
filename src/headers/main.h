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
const int FOOD_SIZE = 20;
const float bounce[2] = {PI - PI / 4, PI + PI / 4};
int NUM_ANTS = 10;
int FOOD_DELAY = 1000;
int SPEED = 1;
int FOOD_DETECTION_RADIUS = 10;
int PHERMONE_DETECTION_RADIUS = 10;
int SIMULATION_TIME = 1;
int PRESENT_FOOD = 3;
bool threads_start = false;
pthread_mutex_t *locks;
pthread_cond_t *food_ready;
pthread_t * thread;
void create_threads(pthread_t *thread);
void readConstants(char *file_name);
void *updateAnt_thread(void *args);
#endif