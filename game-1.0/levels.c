#include "levels.h"

void init_grid(PlayGrid* grid) {
	int i,j;

	for (i = 0; i < grid->x; i++) {
		grid->object_grid[i] = malloc(grid->y * sizeof(playgrid_object));
		grid->square_grid[i] = malloc(grid->y * sizeof(playgrid_object));
	}

	for (i = 0; i < grid->x; i++) {
		for (j = 0; j < grid->y; j++) {
			grid->object_grid[i][j] = EMPTY;
			if (i == 0 || (i == (grid->x - 1)) || j == 0 || (j == (grid->y - 1)))
				grid->square_grid[i][j] = WALL;
			else
				grid->square_grid[i][j] = NORMAL;
		}
	}
	
	grid->object_grid[grid->player_x][grid->player_y] = PLAYER;
}

PlayGrid* level1() {
    PlayGrid *grid = malloc(sizeof(PlayGrid));
	
	grid->x = 10;
	grid->y = 8; 
	grid->player_x = 7;
	grid->player_y = 2; 
	grid->n_rocks = 4;
	
	playgrid_object** object_grid;
	playgrid_square** square_grid;

    object_grid = malloc(grid->x * sizeof(playgrid_object *));
	square_grid = malloc(grid->x * sizeof(playgrid_object *));
	
	grid->object_grid = object_grid;
	grid->square_grid = square_grid;

	init_grid(grid);

    placeROCK(3,4);
    placeROCK(4,5);
    placeROCK(5,6);
    placeROCK(3,2);

    placeWALL(2,5);
    placeWALL(3,5);

    placeGOAL(6,6);
    placeGOAL(5,5);
    placeGOAL(4,4);
    placeGOAL(3,3);

	return grid;
}

PlayGrid* level2() {
    PlayGrid *grid = malloc(sizeof(PlayGrid));
	
	grid->x = 16;
	grid->y = 12; 
	grid->player_x = 7;
	grid->player_y = 2; 
	grid->n_rocks = 10;
	
	playgrid_object** object_grid;
	playgrid_square** square_grid;

    object_grid = malloc(grid->x * sizeof(playgrid_object *));
   	square_grid = malloc(grid->x * sizeof(playgrid_object *));
	
	grid->object_grid = object_grid;
	grid->square_grid = square_grid;
	
	init_grid(grid);
	
    placeROCK(3, 4);
    placeROCK(4, 5);
    placeROCK(5, 6);
    placeROCK(3, 2);
    placeROCK(8, 7);
    placeROCK(8, 9);
    placeROCK(7, 8);
    placeROCK(9, 8);
    placeROCK(14, 9);
    placeROCK(13, 9);
    
    placeWALL(2, 5);
    placeWALL(3, 5);
    placeWALL(1, 10);
    placeWALL(1, 9);
    placeWALL(9, 5);
    placeWALL(8, 5);
    placeWALL(9, 4);
    placeWALL(8, 4);
    placeWALL(7, 4);
    placeWALL(7, 5);

    placeGOAL(6, 6);
    placeGOAL(5, 5);
    placeGOAL(4, 4);
    placeGOAL(3, 3);
    placeGOAL(7, 7);
    placeGOAL(7, 8);
    placeGOAL(7, 9);
    placeGOAL(8, 8);
    placeGOAL(14, 4);
    placeGOAL(14, 3);
	
	return grid;
}

PlayGrid* level3() {
    PlayGrid *grid = malloc(sizeof(PlayGrid));
	
	grid->x = 10;
	grid->y = 8; 
	grid->player_x = 5;
	grid->player_y = 2; 
	grid->n_rocks = 4;
	
	playgrid_object** object_grid;
	playgrid_square** square_grid;

    object_grid = malloc(grid->x * sizeof(playgrid_object *));
	square_grid = malloc(grid->x * sizeof(playgrid_object *));
	
	grid->object_grid = object_grid;
	grid->square_grid = square_grid;

	init_grid(grid);
	
    placeROCK(3, 4);
    placeROCK(4, 5);
    placeROCK(5, 5);
    placeROCK(3, 2);
    
    placeWALL(2, 5);
    placeWALL(3, 5);

    placeGOAL(7, 3);
    placeGOAL(5, 1);
    placeGOAL(2, 4);
    placeGOAL(3, 3);
	
	return grid;
}

PlayGrid* level4() {
    PlayGrid *grid = malloc(sizeof(PlayGrid));
	
	grid->x = 9;
	grid->y = 9; 
	grid->player_x = 6;
	grid->player_y = 7; 
	grid->n_rocks = 5;
	
	playgrid_object** object_grid;
	playgrid_square** square_grid;
	
    object_grid = malloc(grid->x * sizeof(playgrid_object *));
	square_grid = malloc(grid->x * sizeof(playgrid_object *));

	grid->object_grid = object_grid;
	grid->square_grid = square_grid;
	
	init_grid(grid);

    placeROCK(3,2);
    placeROCK(3,6);
    placeROCK(5,2);
    placeROCK(5,3);
    placeROCK(5,6);
    
    placeWALL(1,1);
    placeWALL(1,2);
    placeWALL(1,3);
    placeWALL(1,4);
    placeWALL(1,5);
    placeWALL(2,3);
    placeWALL(3,5);
    placeWALL(4,3);
    placeWALL(4,4);
    placeWALL(4,5);
    placeWALL(6,5);
    placeWALL(7,3);
    placeWALL(7,4);
    placeWALL(7,5);
    placeWALL(7,6);
    placeWALL(7,7);

    placeGOAL(2,6);
    placeGOAL(4,2);
    placeGOAL(5,3);
    placeGOAL(5,6);
    placeGOAL(6,2);

	return grid;
}

