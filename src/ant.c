#include "headers/ant.h"

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

void *updateAnt_thread(void *args)
{
    while (running)
    {
        updateAnt(args, food);
        usleep(1 / 60.0 * 1000000);
    }
    pthread_exit(NULL);
}

void *makeFood(void *arg)
{
    pthread_mutex_init(&food_placment_lock, NULL);
    int INITIAL_SIZE = 3;
    PRESENT_FOOD = 0;
    food = malloc(sizeof(Food) * INITIAL_SIZE);
    bool initial_setup = true;
    while (running)
    {
        // check if a food portion is outside the screen then remove it from the food array
        sleep(FOOD_DELAY);
        pthread_mutex_lock(&food_placment_lock);
        for (int j=0; j < PRESENT_FOOD; j++)
        {
            if (food[j].portionts <= 0)
            {
                for (int i = j; i < PRESENT_FOOD - 1; i++)
                {
                    food[i] = food[i + 1];
                }
                PRESENT_FOOD--;
            }
            food = realloc(food, sizeof(Food) * PRESENT_FOOD);
        }
        if (initial_setup)
            initial_setup = false;
        else
            food = realloc(food, sizeof(Food) * (PRESENT_FOOD + INITIAL_SIZE));
        for (int i = PRESENT_FOOD; i < PRESENT_FOOD + INITIAL_SIZE; ++i)
        {
            pthread_mutex_init(&food[i].lock, NULL);
            food[i].G = rand() % 255;
            food[i].B = rand() % 255;
            food[i].R = rand() % 255;
            food[i].A = rand() % 125 + 125;
            food[i].x = rand() % (SCREEN_WIDTH - SCREEN_WIDTH / 3) + SCREEN_WIDTH / 3;
            food[i].y = rand() % (SCREEN_HEIGHT - SCREEN_HEIGHT / 3) + SCREEN_HEIGHT / 3;
            food[i].portionts = 20;
            food[i].ants_count = 0;
        }
        PRESENT_FOOD += INITIAL_SIZE;
        pthread_mutex_unlock(&food_placment_lock);
    }
    pthread_exit(NULL);
}

void updateAnt(Ant *ant, Food *food)
{
    int min_distance = INT_MAX;
    int index = -1;
    // printf("Ant %d Ph: %d\n", ant->ID, ant->pheromone);
    if (food != NULL)
    {
        pthread_mutex_lock(&food_placment_lock);
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
        pthread_mutex_unlock(&food_placment_lock);
        float dx = food[index].x - ant->x;
        float dy = food[index].y - ant->y;

        if (min_distance < FOOD_DETECTION_RADIUS)
        {
            ant->angle = atan2(dy, dx);
            ant->distance = min_distance;
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
                    pthread_mutex_lock(&food[index].lock);
                    if (food[index].x == INT_MAX || food[index].y == INT_MAX)
                    {
                        pthread_mutex_unlock(&food[index].lock);
                    }
                    else
                    {
                        ant->speed = 0;
                        ant->ate = true;
                        if (food[index].portionts > 0)
                        {
                            food[index].portionts--;
                        }
                        else
                        {
                            pthread_mutex_lock(&food_placment_lock);
                            food[index].x = INT_MAX;
                            food[index].y = INT_MAX;
                            ant->pheromone = 0;
                            for (int i = 0; i < food[index].ants_count; ++i)
                            {
                                pthread_mutex_lock(&food[index].nearby_ants[i]->lock);
                                food[index].nearby_ants[i]->speed = SPEED;
                                food[index].nearby_ants[i]->R = 0;
                                food[index].nearby_ants[i]->G = 0;
                                food[index].nearby_ants[i]->B = 0;
                                food[index].nearby_ants[i]->ate = false;
                                food[index].nearby_ants[i]->pheromone = 0;
                                ant->speed = SPEED;
                                ant->pheromone = 0;
                                pthread_mutex_unlock(&food[index].nearby_ants[i]->lock);
                            }
                            pthread_mutex_unlock(&food_placment_lock);
                        }
                        pthread_mutex_unlock(&food[index].lock);
                    }
                }
            }
        }
        else if (min_distance > FOOD_DETECTION_RADIUS && min_distance < 3000)
        {
            Ant *best_ant = NULL;
            for (int i = 0; i < NUM_ANTS; ++i)
            {
                if (ant[i].ID == ant->ID || ant[i].ID > NUM_ANTS || ant[i].ID < 0)
                {
                    continue;
                }
                if (ant[i].pheromone > 20 && ant[i].pheromone < 100)
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
                    int x3 = best_ant->x + cos(best_ant->angle) * best_ant->distance;
                    int y3 = best_ant->y + sin(best_ant->angle) * best_ant->distance;
                    ant->angle = atan2(y3 - ant->y, x3 - ant->x);
                }
                else if (best_ant->pheromone > PHORMONE_ANGLE_SHIFT_AMOUNT)
                {

                    int x3 = best_ant->x + cos(best_ant->angle) * best_ant->distance;
                    int y3 = best_ant->y + sin(best_ant->angle) * best_ant->distance;
                    float angle = atan2(y3 - ant->y, x3 - ant->x);
                    if (angle > ant->angle)
                    {
                        ant->angle += PI / 72;
                    }
                    else if (angle < ant->angle)
                    {
                        ant->angle -= PI / 72;
                    }
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

