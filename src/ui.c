#include "headers/ui.h"
volatile sig_atomic_t quit_game = false;
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
    // updateAnts(ant, NUM_ANTS, elapsed);
    for (int i = 0; i < NUM_ANTS; ++i)
    {
        renderAnt(&ant[i]);
    }
    if (food != NULL)
        renderFood(food);
    SDL_RenderPresent(renderer);
}
int shutdown()
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
    return 0;
}
Ant makeAnt(int size, int id)
{
    Ant ant = {
        .x = rand() % (SCREEN_WIDTH - SCREEN_WIDTH / 10) + SCREEN_WIDTH / 10,
        .y = rand() % (SCREEN_HEIGHT - SCREEN_HEIGHT / 10) + SCREEN_HEIGHT / 10,
        .speed = (float)SPEED,
        .angle = (rand() % 8 + 1) * PI / 4, 
        .R = 0,
        .G = 0,
        .B = 0,
        .A = 255,
        .ID = id,
        .ate = false,
        .pheromone = 0,
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
void updateAnt(Ant *ant, Food *food)
{
    int min_distance = INT_MAX;
    int index = -1;
    if (food != NULL)
    {
        for (int i = 0; i < PRESENT_FOOD; i++)
        {
            float dx = food[i].x - ant->x;
            float dy = food[i].y - ant->y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < min_distance)
            {
                min_distance = distance;
                index = i;
            }
        }

        float dx = food[index].x - ant->x;
        float dy = food[index].y - ant->y;

        if (min_distance < FOOD_DETECTION_RADIUS)
        {
            ant->angle = atan2(dy, dx);
            if (min_distance - ANT_SIZE/2 <= FOOD_SIZE)
            {
                if (!ant->ate)
                {
                    pthread_mutex_lock(&food[index].lock);
                    if (food[index].x == -1000 || food[index].y == -1000)
                    {
                        pthread_mutex_unlock(&food[index].lock);
                    }
                    else
                    {
                        ant->speed = 0;
                        ant->ate = true;
                        food[index].ants_count++;
                        if (food[index].ants_count == 1)
                        {
                            food[index].nearby_ants = malloc(sizeof(Ant *) * food[index].ants_count);
                            food[index].nearby_ants[0] = ant;
                        }
                        else if (food[index].ants_count > 1)
                        {
                            food[index].nearby_ants = realloc(food[index].nearby_ants, sizeof(Ant *) * food[index].ants_count);
                            food[index].nearby_ants[food[index].ants_count - 1] = ant;
                        }
                        if (food[index].portionts > 0)
                        {
                            food[index].portionts--;
                            /*
                            ant->R = 0;
                            ant->G = 250;
                            ant->B = 125;
                            */
                        }
                        else
                        {
                            pthread_mutex_lock(&food_placment_lock);
                            food[index].x = -1000;
                            food[index].y = -1000;
                            // PRESENT_FOOD--;
                            for (int i = 0; i < food[index].ants_count; ++i)
                            {
                                food[index].nearby_ants[i]->speed = SPEED;
                                food[index].nearby_ants[i]->R = 0;
                                food[index].nearby_ants[i]->G = 0;
                                food[index].nearby_ants[i]->B = 0;
                                food[index].nearby_ants[i]->ate = false;
                            }
                            pthread_mutex_unlock(&food_placment_lock);
                        }
                        pthread_mutex_unlock(&food[index].lock);
                    }
                }
            }
        }
        ant->x += ant->speed * cos(ant->angle);
        ant->y += ant->speed * sin(ant->angle);
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
}