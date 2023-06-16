#include "headers/main.h"
sig_atomic_t cleaned = false;
sig_atomic_t quit = false;
bool running = true;
void clean(void);
void *timer(void *args);
int main(int argc, char *argv[])
{
	atexit(clean);
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	atexit(clean);
	srand(getpid() + time(NULL));
	readConstants(argv[1]);
	thread = malloc(sizeof(pthread_t) * (NUM_ANTS + 2));
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
				quit = true;  // Set the flag to exit the loop naturally
				break;        // Break out of the inner loop
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_SPACE)
				{
					started = true;
				}
			}
		}

		if (quit)
			break;  // Break out of the outer loop if quit flag is set

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

	exit(0);
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
	pthread_create(&thread[NUM_ANTS + 1], NULL, timer, NULL);
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
		shutdown();
	}
}

void *timer(void *args)
{
	sleep(SIMULATION_TIME*60);
	quit = true;
	pthread_exit(NULL);
}