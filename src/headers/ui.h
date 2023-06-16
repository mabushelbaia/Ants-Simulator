#ifndef UI_H
#define UI_H
#include "common.h"
#include "ant.h"
// Globals

bool initialize(void);
void update(Ant *ant, Food *food);
void shutdown(void);
void renderAnt(const Ant *ant);
void renderFood(const Food *food);
void initial_screen(void);
#endif
