#include "headers/ui.h"
#include "headers/main.h"
bool cleaned = false;
bool running = true;
void clean(void);
void *makeFood(void *arg);
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	atexit(clean);
	srand(getpid() + time(NULL));
	readConstants(argv[1]);
	thread = malloc(sizeof(pthread_t) * NUM_ANTS + 1);
	ant = malloc(NUM_ANTS * sizeof(Ant));

	run_gui(thread);
	exit(0);
}

void readConstants(char *file_name)
{
	char path[256];
	sprintf(path, "./data/input/%s", file_name);
	FILE *file = fopen(path, "r");
	if (!file)
	{
		printf("Error: could not open file %s\n", path);
		exit(1);
	}
	char line[256];
	while (fgets(line, sizeof(line), file))
	{
		char *token = strtok(line, " ");
		if (!strcmp(token, "NUMBER_ANTS"))
		{
			token = strtok(NULL, " ");
			NUM_ANTS = atoi(token);
		}
		else if (!strcmp(token, "FOOD_DELAY"))
		{
			token = strtok(NULL, " ");
			FOOD_DELAY = atoi(token);
		}
		else if (!strcmp(token, "SPEED"))
		{
			token = strtok(NULL, " ");
			SPEED = atoi(token);
		}
		else if (!strcmp(token, "FOOD_DETECTION_RADIUS"))
		{
			token = strtok(NULL, " ");
			FOOD_DETECTION_RADIUS = atoi(token);
		}
		else if (!strcmp(token, "PHORMONE_ANGLE_SHIFT_AMOUNT"))
		{
			token = strtok(NULL, " ");
			PHORMONE_ANGLE_SHIFT_AMOUNT = atoi(token);
		}
		else if (!strcmp(token, "PHORMONE_FOLLOWING_RADIUS"))
		{
			token = strtok(NULL, " ");
			PHORMONE_FOLLOWING_RADIUS = atoi(token);
		}
		else if (!strcmp(token, "PHORMONE_DETECTION_THRESS"))
		{
			token = strtok(NULL, " ");
			PHORMONE_DETECTION_THRESS = atoi(token);
		}
		else if (!strcmp(token, "SIMULATION_TIME"))
		{
			token = strtok(NULL, " ");
			SIMULATION_TIME = atoi(token);
		}
		else
		{
			printf("Error: invalid token %s\n", token);
			exit(1);
		}
	}
	fclose(file);
}

void run_gui(pthread_t *thread)
{
	bool status = initialize();
	if (!status)
		exit(1);
	bool quit = false;
	initial_screen();
	SDL_Event event;
	unsigned int lastTick = SDL_GetTicks();
	unsigned int currentTick, elapsed;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				clean();
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_SPACE)
				{
					started = true;
				}
			}
		}

		if (started)
		{
			if (!threads_start)
			{
				threads_start = true;
				create_threads(thread);
			}
			update(ant, food);
		}
	}
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
void create_threads(pthread_t *thread)
{
	printf("Starting threads...\n");
	for (int i = 0; i < NUM_ANTS; ++i)
	{
		ant[i] = makeAnt(ANT_SIZE, i);
	}
	for (int i = 0; i < NUM_ANTS; ++i)
	{
		pthread_create(&thread[i], NULL, updateAnt_thread, (void *)&ant[i]);
	}
	pthread_create(&thread[NUM_ANTS], NULL, makeFood, NULL);
}
void clean(void)
{
	if (!cleaned)
	{
		cleaned = true;
		running = false;
		free(thread);
		free(ant);
		free(food);
		printf("Clean Called\n");
		shutdown();
		exit(0);
	}
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