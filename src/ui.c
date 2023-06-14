#include "headers/ui.h"
// Screen dimension constants
#ifdef __GUI__
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
    for (int i = 0; i < NUM_ANTS; ++i)
    {
        ant[i] = makeAnt(ANT_SIZE);
        // printf("Ant angle: %f\n", ant[i].angle * 180 / PI);
    }
    while (!quit)
    {
        ``m while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT)
            quit = true;
        unsigned int curTick = SDL_GetTicks();
        unsigned int diff = curTick - lastTick;
        float elapsed = diff / 3000.0;
        update(elapsed);
        lastTick = curTick;
    }
    return 0;
}
#endif
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
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    return true;
}

void initial_screen(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render an image here (background)
    SDL_Surface *surface = IMG_Load("src/img/ant-svgrepo-com.png");
    if (!surface)
    {
        printf("Error: could not load image\n%s\n", IMG_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        printf("Error: could not create texture\n");
        SDL_FreeSurface(surface);
        return;
    }

    SDL_FreeSurface(surface);

    // Resize the image to be 300x300
    int size = 200;
    SDL_Rect dstrect = {SCREEN_WIDTH / 2 - size / 2, SCREEN_HEIGHT / 3 - size / 2, size, size};

    // Add text to the bottom of the screen in gray color
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    SDL_Color color = {255 / 2, 255 / 2, 255 / 2}; // Set the color (white in this example)
    TTF_Init();

    TTF_Font *font = TTF_OpenFont("src/fonts/Roboto-Regular.ttf", 24);

    if (!font)
    {
        printf("Error: could not load font\n");
        SDL_DestroyTexture(texture);
        return;
    }
    TTF_SetFontHinting(font, TTF_HINTING_NORMAL);
    surface = TTF_RenderText_Solid(font, "press <space> to start!", color);
    if (!surface)
    {
        printf("Error: could not render text\n");
        SDL_DestroyTexture(texture);
        TTF_CloseFont(font);
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &(SDL_Rect){SCREEN_WIDTH / 2 - surface->w / 2, SCREEN_HEIGHT - 2 * surface->h, surface->w, surface->h});
    font = TTF_OpenFont("src/fonts/Kablammo-Regular-VariableFont_MORF.ttf", 60);
    color = (SDL_Color){0, 0, 0};
    // printf("Surface width: %d\n", surface->w);
    surface = TTF_RenderText_Solid(font, "ANTS SIMULATION", color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &(SDL_Rect){SCREEN_WIDTH/2 - surface->w / 2, dstrect.y + dstrect.h , surface->w, surface->h});
    if (!texture)
    {
        printf("Error: could not create texture for text\n");
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_FreeSurface(surface);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void update(Ant *ant, int NUM_ANTS, float elapsed)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    updateAnts(ant, NUM_ANTS, elapsed);
    renderAnts(ant, NUM_ANTS);
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
        .x = rand() % SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4,
        .y = rand() % SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4,
        .speed = SPEED,
        .angle = (rand() % 8 + 1) * PI / 4, // 45 * [1, 8] == [45, 90, 135, 180, 225, 270, 315, 360]
        .R = 0,
        .G = 0,
        .B = 0,
        .A = 255,
    };
    return ant;
}

void renderAnt(const Ant *ant)
{
    SDL_Rect rect = {
        .x = ant->x - ANT_SIZE / 2,
        .y = ant->y - ANT_SIZE / 2,
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
    if (ant->x + ANT_SIZE / 2 >= SCREEN_WIDTH || ant->x - ANT_SIZE / 2 <= 0)
    {
        if (ant->x + ANT_SIZE / 2 >= SCREEN_WIDTH)
            ant->x = SCREEN_WIDTH - ANT_SIZE / 2;
        else if (ant->x - ANT_SIZE / 2 <= 0)
            ant->x = ANT_SIZE / 2;
        ant->angle = ant->angle + bounce[rand() % 2];
    }
    if (ant->y + ANT_SIZE / 2 >= SCREEN_HEIGHT || ant->y - ANT_SIZE / 2 <= 0)
    {
        if (ant->y + ANT_SIZE / 2 >= SCREEN_HEIGHT)
            ant->y = SCREEN_HEIGHT - ANT_SIZE / 2;
        else if (ant->y - ANT_SIZE / 2 <= 0)
            ant->y = ANT_SIZE / 2;
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

// Create a triangle in SDL
