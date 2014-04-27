#ifndef LEVELS_H
#define LEVELS_H

#include <stdlib.h>
#include "playgrid.h"

#define placeROCK(X,Y) grid->object_grid[X][Y] = ROCK
#define placeWALL(X,Y) grid->square_grid[X][Y] = WALL
#define placeGOAL(X,Y) grid->square_grid[X][Y] = GOAL

PlayGrid* level1();
PlayGrid* level2();
PlayGrid* level3();
PlayGrid* level4();

#endif
