#ifndef UI_H
#define UI_H
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#define PI 3.14159265358979323846

typedef struct Ant
{
    float x;
    float y;
    float speed;
    float angle;
    int R, G, B, A;
} Ant;
// GLobals
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int ANT_SIZE = 20;
const float bounce[2] = { PI -PI / 4, PI + PI / 4};
// Functions
bool initialize(void);
void update(float);
void shutdown(void);
void renderAnt(const Ant *ant);
void updateAnt(Ant *ant, float elapsed);
void updateAnts(Ant * ants, int count, float elapsed);
void renderAnts(Ant * ants, int count);
Ant makeAnt(int size);
#endif