#include "headers/ui.h"
#include "headers/main.h"
void clean(void);
int main(int argc, char *argv[])
{
	atexit(clean);
	printf("Food Detection Radius: %d\n", FOOD_DETECTION_RADIUS);
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	readConstants(argv[1]);
	thread = malloc(sizeof(pthread_t) * NUM_ANTS);
	printf("Displaying constants from file %s\n", argv[1]);
	printf("NUMBER_ANTS: %d\n", NUM_ANTS);
	printf("FOOD_DELAY: %d\n", FOOD_DELAY);
	printf("SPEED: %d\n", SPEED);
	printf("FOOD_DETECTION_RADIUS: %d\n", FOOD_DETECTION_RADIUS);
	printf("PHERMONE_DETECTION_RADIUS: %d\n", PHERMONE_DETECTION_RADIUS);
	srand(getpid() + time(NULL));
	// Create 'NUM_ANTS' threads each thread should run the 'updateAnt' function
	// and each thread should be passed a unique 'Ant' struct
	ant = malloc(NUM_ANTS * sizeof(Ant));
	food = malloc(sizeof(Food) * PRESENT_FOOD);
	locks = malloc(sizeof(pthread_mutex_t) * PRESENT_FOOD);

	for (int i = 0; i < PRESENT_FOOD; ++i)
	{
		pthread_mutex_init(&locks[i], NULL);
	}
	run_gui(thread);
	// gcc file.c -o file -lpthread
	// Destory the muxes
	return 0;
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
		else if (!strcmp(token, "PHERMONE_DETECTION_RADIUS"))
		{
			token = strtok(NULL, " ");
			PHERMONE_DETECTION_RADIUS = atoi(token);
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
				return shutdown();
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
			update(ant, food, NUM_ANTS);
		}
	}
}
void *updateAnt_thread(void *args)
{
	while (true)
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
		ant[i] = makeAnt(ANT_SIZE, SPEED, i);
	}
	for (int i = 0; i < NUM_ANTS; ++i)
	{
		pthread_create(&thread[i], NULL, updateAnt_thread, (void *)&ant[i]);
	}
	for (int i = 0; i < PRESENT_FOOD; ++i)
	{
		food[i].G = rand() % 255;
		food[i].B = rand() % 255;
		food[i].R = rand() % 255;
		food[i].A = 255;
		food[i].x = rand() % (SCREEN_WIDTH - SCREEN_WIDTH / 5) + SCREEN_WIDTH / 5;
		food[i].y = rand() % (SCREEN_HEIGHT - SCREEN_HEIGHT / 5) + SCREEN_HEIGHT / 5;
		food[i].portionts = rand() % 3;
	}
}
void clean(void)
{
	printf("Clean Called");
	for (int i = 0; i < NUM_ANTS; ++i)
	{

		pthread_join(thread[i], NULL);
	}
	for (int i = 0; i < PRESENT_FOOD; i++)
	{
		pthread_mutex_destroy(&locks[i]);
	}
	free(&locks);
	free(ant);
	free(food);
	free(thread);
	shutdown();
}