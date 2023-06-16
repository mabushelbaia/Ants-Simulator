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
const int FOOD_SIZE = 15;
const float bounce[2] = {PI - PI / 4, PI + PI / 4};
int NUM_ANTS;
int FOOD_DELAY;
int SPEED;
int FOOD_DETECTION_RADIUS;
int PHORMONE_DETECTION_THRESS;
int PHORMONE_ANGLE_SHIFT_AMOUNT;
int PHORMONE_FOLLOWING_RADIUS;
int SIMULATION_TIME;
int PRESENT_FOOD;
pthread_mutex_t food_placment_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ant_placment_lock = PTHREAD_MUTEX_INITIALIZER;
bool threads_start = false;
pthread_t * thread;
void create_threads(pthread_t *thread);
void readConstants(char *file_name);
void *updateAnt_thread(void *args);
#endif