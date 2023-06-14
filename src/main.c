#include "headers/common.h"
#include "SDL2/SDL.h"
#include "headers/ui.h"
int NUM_ANTS;
int FOOD_DELAY;
int SPEED;
Ant *ant;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool started = false;
const int SCREEN_WIDTH = 1920/2;
const int SCREEN_HEIGHT = 1080/2;
const int ANT_SIZE = 10;
const float bounce[2] = {PI - PI / 4, PI + PI / 4};
bool readConstants(char * file_name);
int main(int argc, char * argv[]) {

	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	bool status = readConstants(argv[1]);
	if (!status) {
		printf("Error: could not read constants from file %s\n", argv[1]);
		exit(1);
	}
	printf("NUMBER_ANTS: %d\n", NUM_ANTS);
	printf("FOOD_DELAY: %d\n", FOOD_DELAY);
	printf("SPEED: %d\n", SPEED);
	srand(getpid() + time(NULL));
	atexit(shutdown);
	status = initialize();
	if (!status)
		exit(1);
	bool quit = false;
	SDL_Event event;
	unsigned int lastTick = SDL_GetTicks();
	ant = malloc(NUM_ANTS * sizeof(Ant));
	for (int i = 0; i < NUM_ANTS; ++i) {
		ant[i] = makeAnt(ANT_SIZE);
	}
	//gcc file.c -o file -lpthread
	initial_screen();
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = true;
			if (event.type == SDL_KEYDOWN) {
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

	return 0;
}

bool readConstants(char * file_name) {
	char path[256];
	sprintf(path, "./data/input/%s", file_name);
	FILE * file = fopen(path, "r");
	if (!file) {
		printf("Error: could not open file %s\n", path);
		return false;
	}
	char line[256];
	while (fgets(line, sizeof(line), file)) {
		char * token = strtok(line, " ");
		if (!strcmp(token, "NUMBER_ANTS")) {
			token = strtok(NULL, " ");
			NUM_ANTS = atoi(token);
		} else if (!strcmp(token, "FOOD_DELAY")) {
			token = strtok(NULL, " ");
			FOOD_DELAY = atoi(token);
		} else if (!strcmp(token, "SPEED")) {
			token = strtok(NULL, " ");
			SPEED = atoi(token);
		}
	}
	fclose(file);
	return true;
}