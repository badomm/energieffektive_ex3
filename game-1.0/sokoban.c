#define RED    0xF800
#define GREEN  0x07E0
#define BLUE   0x001F
#define YELLOW 0xFFE0
#define BLACK  0x0000
#define WHITE  0xFFFF
#define CYAN   0x07FF
#define PINK   0xf81f


#include "sokoban.h"
#include "screen.h"
#include "playgrid.h"


static int x_tile_width;
static int y_tile_width;
short get_square_graphics(playgrid_object object, playgrid_square square);

PlayGrid* init_sokoban(){

	int x, y, player_x, player_y, n_rocks;

	x = 10;
	y = 8;

	player_x = 7;
	player_y = 2;

	n_rocks = 4;
	

	///////
	//Init playing field:

	playgrid_object** object_grid;
	object_grid = malloc(10 * sizeof(playgrid_object *));
	playgrid_object** square_grid;
	square_grid = malloc(10 * sizeof(playgrid_object *));

	int i,j;

	for (i = 0; i < x; i++) {
		object_grid[i] = malloc(10 * sizeof(playgrid_object));
		square_grid[i] = malloc(10 * sizeof(playgrid_object));
	}

	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			object_grid[i][j] = EMPTY;
			if (i == 0 || (i == (x-1)) || j == 0 || (j == (y-1)))
				square_grid[i][j] = WALL;
			else
				square_grid[i][j] = NORMAL;
		}
	}


	object_grid[3][4] = ROCK;
	object_grid[4][5] = ROCK;
	object_grid[5][6] = ROCK;
	object_grid[3][2] = ROCK;
	object_grid[player_x][player_y] = PLAYER;


	square_grid[2][5] = WALL;
	square_grid[3][5] = WALL;

	square_grid[6][6] = GOAL;
	square_grid[5][5] = GOAL;
	square_grid[4][4] = GOAL;
	square_grid[3][3] = GOAL;
	
	//End Init Playing field
	///////////////

	PlayGrid *grid = malloc(sizeof(PlayGrid));
	grid->x = x;
	grid->y = y; 
	grid->player_x = player_x;
	grid->player_y = player_y; 
	grid->n_rocks = n_rocks;
	grid->object_grid = object_grid;
	grid->square_grid = square_grid;

	y_tile_width = SCREEN_Y / grid->y; // TODO x og y er stygt
	x_tile_width = SCREEN_X / grid->x; // TODO samme

	return grid;
}

void delete_sokoban(PlayGrid* grid){
	int i;
	for(i=0;i<grid->x;i++){
		free(grid->object_grid[i]);
		free(grid->square_grid[i]);
	}
	free(grid->object_grid);
	free(grid->square_grid);
	free(grid);
}

short sokoban_color(PlayGrid *grid, int pixel_i, int pixel_j) {
	int x = pixel_i / x_tile_width;
	int y = pixel_j / y_tile_width;

	if ((y == grid->player_x) && (x == grid->player_y)) {
		if (grid->square_grid[y][x] == NORMAL)
			return BLUE;
		else if (grid->square_grid[y][x] == GOAL)
			return BLACK;
	}
	if (grid->square_grid[y][x] == WALL)
		return RED;
	if (grid->square_grid[y][x] == GOAL) {
		if (grid->object_grid[y][x] == EMPTY)
			return YELLOW;
		else if (grid->object_grid[y][x] == ROCK)
			return GREEN;
	}
	if (grid->square_grid[y][x] == NORMAL) {
		if (grid->object_grid[y][x] == EMPTY)
			return PINK;
		else if (grid->object_grid[y][x] == ROCK)
			return CYAN;
	}
	return WHITE;
}

void grid2screen(PlayGrid *grid) {
	playgrid_object object; 
	playgrid_object square;
	int x,y;
	//loop through squares
	for(x = 0;x < grid->x; x++){
		for(y = 0; y < grid->y; y++){
			//sokoban_draw_square(grid,x,y)
			object = grid->object_grid[x][y];
			square = grid->square_grid[x][y];
			int origo_i = x*x_tile_width;
			int origo_j = y*y_tile_width;
			short color = get_square_graphics(object, square);

			//loop through pixels of square
			int i,j;	
			for(i = 0; i < x_tile_width; i++){
				for(j = 0; j < y_tile_width; j++){
					write2pixel(origo_i+i, origo_j+j, color);	
				}
			}
		}
	}
	update_screen_area(0, 0, 320, 240);
}

short get_square_graphics(playgrid_object object, playgrid_square square){
	switch(square){
		case NORMAL:
			switch(object){
				case EMPTY: return PINK; break;
				case PLAYER: return BLUE; break;
				case ROCK: return CYAN; break;
			}
		break;

		case GOAL:
			switch(object){
				case EMPTY: return YELLOW; break;
				case PLAYER: return BLACK; break;
				case ROCK: return GREEN; break;
			}
		break;

		case WALL:
			switch(object){
				case EMPTY: return RED; break;
				case PLAYER: return WHITE; break;
				case ROCK: return WHITE; break;
			}
		break;
	}
	return WHITE;
}

void sokoban_draw_square(PlayGrid* grid, int x, int y){
	playgrid_object object = grid->object_grid[x][y];
	playgrid_object square = grid->square_grid[x][y];
	
	//calculate square origo position
	int origo_i = x*x_tile_width;
	int origo_j = y*y_tile_width+1;
	
	//write the pixels
	short color = get_square_graphics(object,square);
	int i,j;	
	for(i = 0; i <= x_tile_width; i++){
		for(j = 0; j <= y_tile_width; j++){
			
			write2pixel(origo_i+i, origo_j+j, color);	
		}
	}
	//Update the screen
	update_screen_area(origo_i, origo_j, x_tile_width, y_tile_width);
}



void sokoban_update(playgrid_direction direction,PlayGrid* grid){
	//init list of updated squares	
	Square_list updated;
	updated.n = 0;
	
	//move player in grid
	if(move_object(grid->player_x, grid->player_y, direction, grid, &updated) == LEGAL)
		printf("LEGAL\n");
	else
		printf("ILLEGAL\n");
	//update screen
	int i;
	printf("Updated: %d\n",updated.n);
	for(i = 0; i < updated.n; i++){
		sokoban_draw_square(grid, updated.x[i],updated.y[i]);
	}

}
