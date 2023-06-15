#ifndef UI_H
#define UI_H
#include "common.h"
typedef struct Ant
{
    int ID;
    float x;
    float y;
    float speed;
    float angle;
    int R, G, B, A;
} Ant;
typedef struct Food
{
    float x;
    float y;
    int portionts;
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
extern int PHERMONE_DETECTION_RADIUS;
extern int SIMULATION_DELAY;
extern int PRESENT_FOOD;
extern const float bounce[2];
extern pthread_mutex_t *locks;
extern pthread_cond_t *food_ready;
// Functions
bool initialize(void);
void update(Ant *ant, Food *food, int NUM_ANTS);
int shutdown(void);
void renderAnt(const Ant *ant);
void updateAnt(Ant *ant, Food *food);
void renderAnts(Ant *ants, int count);
void renderFood(const Food *food);
void initial_screen(void);
Ant makeAnt(int size, int speed, int id);

#endif
