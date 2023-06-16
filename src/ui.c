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
    pthread_mutex_init(&ant.lock, NULL);
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
    // printf("Ant %d Ph: %d\n", ant->ID, ant->pheromone);
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
            ant->pheromone = (int)100 * exp(-0.00356674944 * min_distance);
            pthread_mutex_lock(&food[index].lock);
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
            pthread_mutex_unlock(&food[index].lock);
            if (min_distance - ANT_SIZE / 2 <= FOOD_SIZE)
            {
                if (!ant->ate)
                {
                    ant->speed = 0;
                    ant->ate = true;
                    if (food[index].portionts > 0)
                    {
                        food[index].portionts--;
                    }
                }
            }
        }

        else
        {
            Ant *best_ant = NULL;
            for (int i = 0; i < NUM_ANTS; ++i)
            {
                if (ant[i].ID == ant->ID || ant[i].ID > NUM_ANTS || ant[i].ID < 0)
                {
                    continue;
                }
                if (ant[i].pheromone > 0 && ant[i].pheromone < 100)
                {
                    if (best_ant == NULL)
                    {
                        best_ant = &ant[i];
                    }
                    else if (ant[i].pheromone > best_ant->pheromone)
                    {
                        best_ant = &ant[i];
                    }
                    else if (ant[i].pheromone == best_ant->pheromone)
                    {
                        int distance1 = sqrt((food[index].x - ant[i].x) * (food[index].x - ant[i].x) + (food[index].y - ant[i].y) * (food[index].y - ant[i].y));
                        int distance2 = sqrt((food[index].x - best_ant->x) * (food[index].x - best_ant->x) + (food[index].y - best_ant->y) * (food[index].y - best_ant->y));
                        if (distance1 < distance2)
                        {
                            best_ant = &ant[i];
                        }
                    }
                }
            }
            if (best_ant != NULL)
            {
                int ants_distance = sqrt((ant->x - best_ant->x) * (ant->x - best_ant->x) + (ant->y - best_ant->y) * (ant->y - best_ant->y));
                if (ants_distance < PHORMONE_FOLLOWING_RADIUS)
                {
                    ant->angle = atan2(best_ant->y - ant->y, best_ant->x - ant->x);
                    ant->R = 255;
                    ant->G = 125;
                    ant->B = 0;
                    printf("Ant %d following ant %d\n", ant->ID, best_ant->ID);
                    printf("Ant %d pheromone %d\n", ant->ID, ant->pheromone);
                    printf("Ant %d pheromone %d\n", best_ant->ID, best_ant->pheromone);
                    best_ant->R = 255;
                }
                else
                {
                    ant->R = 0;
                    ant->G = 0;
                    ant->B = 0;
                }
            }
            best_ant = NULL;
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
