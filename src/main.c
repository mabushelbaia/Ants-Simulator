#include "headers/common.h"
int NUMBER_ANTS;
int FOOD_DELAY;
int SPEED;
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
			NUMBER_ANTS = atoi(token);
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