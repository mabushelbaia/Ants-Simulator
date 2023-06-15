#include "headers/ui.h"
#include "headers/main.h"
bool cleaned = false;
bool running = true;
void clean(void);
void *createPortions(void *arg);
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
	// locks = malloc(sizeof(pthread_mutex_t) * PRESENT_FOOD);
	// conds = malloc(sizeof(pthread_cond_t) * PRESENT_FOOD);

	// for (int i = 0; i < PRESENT_FOOD; ++i)
	// {
	// 	pthread_mutex_init(&locks[i], NULL);
	// 	//pthread_cond_init(&conds[i], NULL);
	// }
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
	pthread_create(&thread[NUM_ANTS], NULL, createPortions, NULL);
}
void clean(void)
{
	if (!cleaned)
	{
		cleaned = true;
		running = false;
		printf("Clean Called\n");
		shutdown();
		exit(0);
	}
}

void *createPortions(void *arg)
{
	int current_size = PRESENT_FOOD;
	int counter = 1071;
	food = malloc(sizeof(Food) * PRESENT_FOOD);
	while (running)
	{
		for (int i = 0; i < PRESENT_FOOD; ++i)
		{
			if (food[i].x == -1000 && food[i].y == -1000) {
				
			}
			food[i].id = 0;
			food[i].G = rand() % 255;
			food[i].B = rand() % 255;
			food[i].R = rand() % 255;
			food[i].A = rand() % 125 + 125;
			food[i].x = rand() % (SCREEN_WIDTH - SCREEN_WIDTH / 3) + SCREEN_WIDTH / 3;
			food[i].y = rand() % (SCREEN_HEIGHT - SCREEN_HEIGHT / 3) + SCREEN_HEIGHT / 3;
			food[i].portionts = rand() % 3 + 1;
			food[i].eaten_by = malloc(sizeof(int) * NUM_ANTS);
			pthread_mutex_init(&food[i].lock, NULL);
		}
		sleep(FOOD_DELAY * 100);
	}
	pthread_exit(NULL);
}