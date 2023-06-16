#include "headers/ui.h"
volatile sig_atomic_t quit_game = false;
bool visited = false;
bool initialize(void)
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("Ants Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    int size = 200;
    SDL_Rect dstrect = {SCREEN_WIDTH / 2 - size / 2, SCREEN_HEIGHT / 3 - size / 2, size, size};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    SDL_Color color = {255 / 2, 255 / 2, 255 / 2};
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
    SDL_RenderCopy(renderer, texture, NULL, &(SDL_Rect){SCREEN_WIDTH / 2 - surface->w / 2, dstrect.y + dstrect.h, surface->w, surface->h});
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

void update(Ant *ant, Food *food)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < NUM_ANTS; ++i)
    {
        renderAnt(&ant[i]);
    }
    if (food != NULL)
        renderFood(food);
    SDL_RenderPresent(renderer);
}
void shutdown()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    if (!quit_game)
    {
        quit_game = true;
        printf("Quitting SDL...\n");
        SDL_Quit();
    }
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
void renderFood(const Food *food)
{
    for (int i = 0; i < PRESENT_FOOD; i++)
    {
        for (int y = -FOOD_SIZE; y <= FOOD_SIZE; y++)
        {
            for (int x = -FOOD_SIZE; x <= FOOD_SIZE; x++)
            {
                if (x * x + y * y <= FOOD_SIZE * FOOD_SIZE)
                {
                    int drawX = food[i].x + x;
                    int drawY = food[i].y + y;
                    SDL_SetRenderDrawColor(renderer, food[i].R, food[i].G, food[i].B, food[i].A);
                    SDL_RenderDrawPoint(renderer, drawX, drawY);
                }
            }
        }
    }
}
