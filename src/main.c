#include "SDL2/SDL.h"
#include "headers/ui.h"
#include "headers/common.h"
int NUM_ANTS = 300;
int FOOD_DELAY = 10;
int SPEED = 7;
int FOOD_DETECTION_RADIUS = 3;
int PHERMONE_DETECTION_RADIUS = 10;
Ant *ant;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool started = false;
const int SCREEN_WIDTH = 1920 / 2;
const int SCREEN_HEIGHT = 1080 / 2;
const int ANT_SIZE = 10;
const float bounce[2] = {PI - PI / 4, PI + PI / 4};
void readConstants(char *file_name);

int main(int argc, char *argv[])
{

	atexit(shutdown);
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	readConstants(argv[1]);
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
	for (int i = 0; i < NUM_ANTS; ++i)
	{
		ant[i] = makeAnt(ANT_SIZE);
	}
	// gcc file.c -o file -lpthread
	run_gui();
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
	}
	fclose(file);
}

void run_gui()
{
	bool status = initialize();
	if (!status)
		exit(1);
	bool quit = false;
	initial_screen();
	SDL_Event event;
	unsigned int lastTick = SDL_GetTicks();
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
					started = true;
			}
		}
		unsigned int curTick = SDL_GetTicks();
		unsigned int diff = curTick - lastTick;
		float elapsed = diff / 3000.0;
		if (started)
			update(ant, NUM_ANTS, elapsed);
		lastTick = curTick;
	}
}