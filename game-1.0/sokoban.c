#include "sokoban.h"

static int x_tile_width;
static int y_tile_width;

static int level = 1;
static int won;

short get_square_graphics(playgrid_object object, playgrid_square square);
int is_winning(PlayGrid* grid);
void sokoban_draw_square(PlayGrid* grid, int x, int y);

PlayGrid* init_sokoban() {
    PlayGrid* grid;
    switch(level) {
        case 1: grid = level1();
            break;
        case 2: grid = level2();
            break;
        case 3: grid = level3();
            break;
        case 4: grid = level4();
            break;
        default: grid = level1();
            break;
    }
    
    won = 0;
	
	x_tile_width = SCREEN_X / grid->x;
    y_tile_width = SCREEN_Y / grid->y;
    
    return grid;
}

void sokoban_update(playgrid_direction direction,PlayGrid* grid){
    // Did we already win?
    if (won)
        return;

	//init list of updated squares	
	Square_list updated;
	updated.n = 0;
	
	//move player in grid
	(void)move_object(grid->player_x, grid->player_y, direction, grid, &updated);

	//update screen
	int i;

	for(i = 0; i < updated.n; i++){
		sokoban_draw_square(grid, updated.x[i],updated.y[i]);
	}
	
    if (is_winning(grid)) {
        grid2screen(grid);
        return;
    }
}

int is_winning(PlayGrid* grid) {
    int i, j;
    
    for (i = 0; i < grid->x; i++) {
        for (j = 0; j < grid->y; j++) {
            if (grid->square_grid[i][j] == GOAL && !(grid->object_grid[i][j] == ROCK)) {
                return 0;
            }
        }
    }
    won = 1;
    return 1;
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
				case EMPTY: {
				    if (won)
				        return GREEN;
				    else
				        return RED;
				    break;
				}
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
	for(i = 0; i <= x_tile_width; i++) {
		for(j = 0; j <= y_tile_width; j++) {
			write2pixel(origo_i+i, origo_j+j, color);	
		}
	}
	//Update the screen
	update_screen_area(origo_i, origo_j, x_tile_width, y_tile_width);
}

void sokoban_level_down() {
    if (level > 1)
        --level;
    return;
}

void sokoban_level_up() {
    if (level < 4)
        ++level;
    return;
}

