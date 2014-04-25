#include "playgrid.h"

playgrid_square get_Playgrid_square(int x,int y, PlayGrid* grid){
	return grid->square_grid[x][y];
}
playgrid_object get_Playgrid_object(int x, int y, PlayGrid* grid){
	return grid->object_grid[x][y];
}

playgrid_legal move_object(int x, int y, playgrid_direction direction, PlayGrid* grid){
	int x_new = x;
	int y_new = y;

	playgrid_object current_object;
        playgrid_square current_square;
	playgrid_object new_object;
	playgrid_square new_square;

	//based on moving direction, calculate new positions
	switch(direction){
		case RIGHT: {
			y_new += 1;
			break;
		}
		case LEFT: {
			y_new -= 1;
			break;
		}
		case UP: {
			x_new -= 1;
			break;
		}
		case DOWN: {
			x_new += 1;
			break;
		}
	}

	//check if new values are within legal range (within grid)
	if(y_new < 0 || y_new >= grid->y ||
	   x_new < 0 || x_new >= grid->x){
		return ILLEGAL;
	}

	//get objects and squares for new and current position
	current_object = get_Playgrid_object(x,y,grid);
        current_square = get_Playgrid_square(x,y,grid);
        new_object = get_Playgrid_object(x_new,y_new,grid);
        new_square = get_Playgrid_square(x_new,y_new,grid);

	//if new_square is a wall, not legal move
	switch(new_square){
		case WALL: { return ILLEGAL; break; }
		case GOAL:
		case NORMAL:
		default: break;
	}

	return LEGAL;
}

playgrid_legal move_rock(int x,int y, int x_new, int y_new, PlayGrid* grid){
	return LEGAL;
}
playgrid_legal move_player(playgrid_direction direction);
