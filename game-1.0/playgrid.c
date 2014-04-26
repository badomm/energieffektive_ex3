#include "playgrid.h"
#include <stdio.h>
playgrid_legal move_rock(int x,int y, int x_new, int y_new, PlayGrid* grid);
playgrid_legal move_player(playgrid_direction direction, int x_new, int y_new, PlayGrid* grid);

playgrid_square get_Playgrid_square(int x,int y, PlayGrid* grid){
	return grid->square_grid[x][y];
}
playgrid_object get_Playgrid_object(int x, int y, PlayGrid* grid){
	return grid->object_grid[x][y];
}

playgrid_legal move_object(int x, int y, int* x_out, int* y_out, playgrid_direction direction, PlayGrid* grid){	
	int x_new = x;
	int y_new = y;

	playgrid_object current_object;
        playgrid_square current_square;
	
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
	
	//update the output
	*x_out = x_new;
	*y_out = y_new;	
	
	//check if new values are within legal range (within grid)
	if(y_new < 0 || y_new >= grid->y ||
	   x_new < 0 || x_new >= grid->x){
		return ILLEGAL;
	}

	//get objects and squares for new and current position
	current_object = get_Playgrid_object(x,y,grid);
        current_square = get_Playgrid_square(x,y,grid);
        
        new_square = get_Playgrid_square(x_new,y_new,grid);

	//if new_square is a wall, not legal move
	switch(new_square){
		case WALL: { return ILLEGAL; break; }
		case GOAL:
		case NORMAL:
		default: break;
	}
	
	//move depends on object being moved
	switch(current_object){
		case EMPTY:	//cannot move something empty
			return ILLEGAL;		
		break;	
		case PLAYER:
			return move_player(direction, x_new, y_new, grid);
		break;	
		case ROCK:
			return move_rock(x,y,x_new,y_new,grid);		
		break;
		
		default:
			printf("moving object, DEFAULT CASE\n");
			return ILLEGAL;		
		break;
	}
	
	return LEGAL;
}

playgrid_legal move_rock(int x,int y, int x_new, int y_new, PlayGrid* grid){
	playgrid_object new_object;	//object at new square
	
	//is there already a rock at the new square?
	new_object = get_Playgrid_object(x_new,y_new,grid);
	switch(new_object){
		case ROCK:
			return ILLEGAL;
		break;
		case EMPTY:
			//move object:
			
		break;		
		default:
			return ILLEGAL;
		break;
	}
	
	//everything seems fine, move rock
	grid->object_grid[x][y] = EMPTY;
	grid->object_grid[x_new][y_new]  = ROCK;
	
	return LEGAL;
}

playgrid_legal move_player(playgrid_direction direction, int x_new, int y_new, PlayGrid* grid){
	playgrid_object new_object;	//object at new square
	
	new_object = get_Playgrid_object(x_new,y_new,grid);
	
	//is the new square legal to move to	
	switch(new_object){
		case ROCK://try to move the rock
			if( ILLEGAL == move_object(x_new, y_new, direction, grid) ){
				return ILLEGAL;			
			}
		break;

		case EMPTY://EMPTY is fine
		break;		
			
		default:
			return ILLEGAL;
		break;
	}
	//everything seems fine, move player

	grid->object_grid[grid->player_x][grid->player_y] = EMPTY;
	grid->object_grid[x_new][y_new]  = PLAYER;
	
	grid->player_x = x_new;
	grid->player_y = y_new;

	return LEGAL;
}
