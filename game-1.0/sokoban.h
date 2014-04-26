#ifndef SOKOBAN_H
#define SOKOBAN_H

#include <stdio.h>
#include <stdlib.h>
#include "playgrid.h"
#include "screen.h"



PlayGrid* init_sokoban();
void grid2screen(PlayGrid *grid);
void sokoban_update(playgrid_direction direction,PlayGrid* grid);
void delete_sokoban(PlayGrid* grid);

#endif
