#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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

// Globals
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int ANT_SIZE;
extern const float bounce[2];
// Functions
bool initialize(void);
void update(Ant *ant, int NUM_ANTS, float elapsed);
void shutdown(void);
void renderAnt(const Ant *ant);
void updateAnt(Ant *ant, float elapsed);
void updateAnts(Ant *ants, int count, float elapsed);
void renderAnts(Ant *ants, int count);
void initial_screen(void);
Ant makeAnt(int size);

#endif
