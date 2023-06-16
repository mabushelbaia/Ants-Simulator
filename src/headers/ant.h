#ifndef ANT_H
#define ANT_H
#include "common.h"
extern int NUM_ANTS;
extern int FOOD_DELAY;
extern int SPEED;
extern int FOOD_DETECTION_RADIUS;
extern int PHORMONE_ANGLE_SHIFT_AMOUNT;
extern int PHORMONE_FOLLOWING_RADIUS;
extern int PHORMONE_DETECTION_THRESS;
extern int SIMULATION_DELAY;
extern int PRESENT_FOOD;
extern const float bounce[2];
extern pthread_mutex_t food_placment_lock;
extern pthread_mutex_t ant_placment_lock;
extern bool running;
void updateAnt(Ant *ant, Food *food);
Ant makeAnt(int size, int id);
void *makeFood(void *arg);
void *updateAnt_thread(void *args);
#endif
