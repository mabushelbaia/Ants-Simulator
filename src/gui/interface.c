// Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#define PI 3.141592654
typedef struct Ant {
    float x;
    float y;
    float x_speed;
    float y_speed;
    int size;
} Ant;
Ant ant;
// Screen dimension constants
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int ANT_SIZE = 10;
bool initialize(void);
void update(float);
void shutdown(void);
void renderAnt(const Ant *ant);
void updateAnt(Ant* ant, float elapsed);
Ant makeAnt(int size);
int main(int argc, char *args[])
{
    atexit(shutdown);
    bool status = initialize();
    if (!status) exit(1);
    bool quit = false;
    SDL_Event event;
    unsigned int lastTick = SDL_GetTicks();
    ant = makeAnt(ANT_SIZE);
    while (!quit)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT) quit = true;
        unsigned int curTick = SDL_GetTicks();
        unsigned int diff = curTick - lastTick;
        float elapsed = diff / 1000.0f;

        update(elapsed);
        lastTick = curTick;
    }
    return 0;
}
bool initialize(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("SDL ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!window)
        return false;
    return true;
}

void update(float elapsed)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    updateAnt(&ant, elapsed);
    renderAnt(&ant);
    SDL_RenderPresent(renderer);
}
void shutdown(void)
{
    if (renderer == NULL)
        SDL_DestroyRenderer(renderer);
    if (window == NULL)
        SDL_DestroyWindow(window);
    SDL_Quit();
}
Ant makeAnt(int size) {
    const float SPEED = 120;
    Ant ant = {
        .x = SCREEN_WIDTH / 2 - size/2, 
        .y = SCREEN_HEIGHT / 2 - size/2,
        .size = size,
        .x_speed = SPEED,
        .y_speed = SPEED,
    };
    return ant;
}

void renderAnt(const Ant *ant) {
    int size = ant->size;
    int halfSize = size / 2;
    SDL_Rect rect = {
        .x = ant->x - halfSize,
        .y = ant->y - halfSize,
        .w = size,
        .h = size,
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void updateAnt(Ant* ant, float elapsed){
    ant-> x += ant->x_speed * elapsed;
    ant-> y += ant->y_speed * elapsed;
    int x_cor = ant->x + ant->size /2;
    int y_cor = ant->y + ant->size /2;
    if (x_cor >= SCREEN_WIDTH || x_cor <= 0) {
        ant->x_speed *= -1;
    }
    if (y_cor >= SCREEN_HEIGHT || y_cor <= 0) {
        ant->y_speed *= -1;
    }
}