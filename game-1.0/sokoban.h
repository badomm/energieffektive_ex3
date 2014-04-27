#ifndef SOKOBAN_H
#define SOKOBAN_H

#include <stdio.h>
#include <stdlib.h>
#include "playgrid.h"
#include "screen.h"
#include "levels.h"

#define RED    0xF800
#define GREEN  0x07E0
#define BLUE   0x001F
#define YELLOW 0xFFE0
#define BLACK  0x0000
#define WHITE  0xFFFF
#define CYAN   0x07FF
#define PINK   0xf81f

PlayGrid* init_sokoban();
void grid2screen(PlayGrid *grid);
void sokoban_update(playgrid_direction direction,PlayGrid* grid);
void delete_sokoban(PlayGrid* grid);
void sokoban_level_down();
void sokoban_level_up();

#endif
