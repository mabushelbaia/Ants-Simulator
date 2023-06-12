#include "headers/ui.h"
Ant ant[10];
// Screen dimension constants

int main(int argc, char *args[])
{
    srand(getpid() + time(NULL));
    atexit(shutdown);
    bool status = initialize();
    if (!status)
        exit(1);
    bool quit = false;
    SDL_Event event;
    unsigned int lastTick = SDL_GetTicks();
    for (int i = 0; i < 10; ++i)
    {
        ant[i] = makeAnt(ANT_SIZE);
        //printf("Ant angle: %f\n", ant[i].angle * 180 / PI);
    }
    while (!quit)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                quit = true;
        unsigned int curTick = SDL_GetTicks();
        unsigned int diff = curTick - lastTick;
        float elapsed = diff/ 3000.0;
        update(elapsed);
        lastTick = curTick;
    }
    return 0;
}
bool initialize(void)
{
    if (SDL_Init(SDL_INIT_VIDEO))
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    updateAnts(ant, 10, elapsed);
    renderAnts(ant, 10);
    SDL_RenderPresent(renderer);
}
void shutdown(void)
{
    if (renderer == NULL)
        SDL_DestroyRenderer(renderer);
    if (window == NULL)
        SDL_DestroyWindow(window);
}
Ant makeAnt(int size)
{
    const float SPEED = 500.0;
    Ant ant = {
        .x = rand() % SCREEN_WIDTH/2 + SCREEN_WIDTH/4,
        .y = rand() % SCREEN_HEIGHT/2 + SCREEN_HEIGHT/4,
        .speed = SPEED,
        .angle = (rand() % 8 + 1) * PI / 4, // 45 * [1, 8] == [45, 90, 135, 180, 225, 270, 315, 360]
        .R = rand() % 255,
        .G = rand() % 255,
        .B = rand() % 255,
        .A = 255,
    };
    return ant;
}

void renderAnt(const Ant *ant)
{
    SDL_Rect rect = {
        .x = ant->x - ANT_SIZE/2,
        .y = ant->y - ANT_SIZE/2,
        .w = ANT_SIZE,
        .h = ANT_SIZE,
    };
    SDL_SetRenderDrawColor(renderer, ant->R, ant->G, ant->B, ant->A);
    SDL_RenderFillRect(renderer, &rect);
}

void updateAnt(Ant *ant, float elapsed)
{
    ant->x += ant->speed * cos(ant->angle) * elapsed;
    ant->y += ant->speed * sin(ant->angle) * elapsed;
    if (ant->x + ANT_SIZE/2 >= SCREEN_WIDTH || ant->x - ANT_SIZE/2 <= 0)
    {
        if (ant-> x + ANT_SIZE/2 >= SCREEN_WIDTH) ant-> x = SCREEN_WIDTH - ANT_SIZE/2;
        else if (ant->x - ANT_SIZE/2 <= 0) ant-> x = ANT_SIZE/2;
        ant->angle = ant->angle + bounce[rand() % 2];
    }
    if (ant->y + ANT_SIZE/2 >= SCREEN_HEIGHT || ant->y - ANT_SIZE/2 <= 0)
    {
        if (ant-> y + ANT_SIZE/2 >= SCREEN_HEIGHT) ant-> y = SCREEN_HEIGHT - ANT_SIZE/2;
        else if (ant->y - ANT_SIZE/2 <= 0) ant-> y = ANT_SIZE/2;
        ant->angle = ant->angle + bounce[rand() % 2];
    }
}

void updateAnts(Ant *ant, int count, float elapsed)
{
    for (int i = 0; i < count; ++i)
    {
        updateAnt(&ant[i], elapsed);
    }
}
void renderAnts(Ant *ant, int count)
{
    for (int i = 0; i < count; ++i)
    {
        renderAnt(&ant[i]);
    }
}
