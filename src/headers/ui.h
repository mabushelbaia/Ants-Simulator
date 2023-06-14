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
    int size;
    int R, G, B, A;
} Food;
// Globals
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int ANT_SIZE;
extern const int FOOD_SIZE;
extern const int NUM_ANTS;
extern const int FOOD_DELAY;
extern const int SPEED;
extern const int FOOD_DETECTION_RADIUS;
extern const int PHERMONE_DETECTION_RADIUS;
extern const float bounce[2];
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
