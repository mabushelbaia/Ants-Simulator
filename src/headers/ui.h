#ifndef UI_H
#define UI_H
#include "common.h"
typedef struct Ant
{
    float x;
    float y;
    float speed;
    float angle;
    bool ate;
    int ID;
    int R, G, B, A;
    int pheromone;
    int distance;
    pthread_mutex_t lock;
} Ant;
typedef struct Food
{
    int id;
    float x;
    float y;
    int portionts;
    pthread_mutex_t lock;
    int ants_count;
    Ant **nearby_ants;
    int R, G, B, A;
} Food;
// Globals
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int ANT_SIZE;
extern const int FOOD_SIZE;
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
// Functions
bool initialize(void);
void update(Ant *ant, Food *food);
int shutdown(void);
void renderAnt(const Ant *ant);
void updateAnt(Ant *ant, Food *food);
void renderFood(const Food *food);
void initial_screen(void);
Ant makeAnt(int size, int id);
void *makeFood(void *arg);
#endif
