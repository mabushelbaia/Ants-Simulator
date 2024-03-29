#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

// Macros
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Constants
#define PI 3.14159265358979323846
#define E 2.71828182845904523536

// Typedefs
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;
typedef long long llong;
typedef unsigned char uchar;
typedef unsigned short ushort;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int ANT_SIZE;
extern const int FOOD_SIZE;
typedef struct Ant
{
    float x;
    float y;
    float speed;
    float angle;
    bool ate;
    int ID;
    int R, G, B, A;
    int pheromone;
    int distance;
    int counter;
    pthread_mutex_t lock;
} Ant;

typedef struct Food
{
    int id;
    float x;
    float y;
    int portionts;
    pthread_mutex_t lock;
    int ants_count;
    Ant **nearby_ants;
    int R, G, B, A;
} Food;
extern Food *food;
extern Ant *ant;
#endif