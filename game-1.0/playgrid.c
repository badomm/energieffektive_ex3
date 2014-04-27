#include "playgrid.h"
#include <stdio.h>
#include <stdlib.h>
playgrid_legal move_rock(int x,int y, int x_new, int y_new, PlayGrid* grid, Square_list* updated);
playgrid_legal move_player(playgrid_direction direction, int x_new, int y_new, PlayGrid* grid, Square_list* updated);

playgrid_square get_Playgrid_square(int x,int y, PlayGrid* grid){
	return grid->square_grid[x][y];
}
playgrid_object get_Playgrid_object(int x, int y, PlayGrid* grid){
	return grid->object_grid[x][y];
}

void update_square(PlayGrid* grid, playgrid_object object,int x, int y, Square_list *updated){
	grid->object_grid[x][y] = object;
	updated->x[updated->n] = x;
	updated->y[updated->n] = y;
	updated->n++;

	if(object == PLAYER){
		grid->player_x = x;
		grid->player_y = y;
	}
}

playgrid_legal move_object(int x, int y, playgrid_direction direction, PlayGrid* grid, Square_list* updated){	
	int x_new = x;
	int y_new = y;

	playgrid_object current_object;
	
	playgrid_square new_square;

	//based on moving direction, calculate new positions
	switch(direction){
		case RIGHT: {
			x_new += 1;
			break;
		}
		case LEFT: {
			x_new -= 1;
			break;
		}
		case UP: {
			y_new -= 1;
			break;
		}
		case DOWN: {
			y_new += 1;
			break;
		}
		default:
			return ILLEGAL;
		break;
	}
	
	printf("move_object: move legal\n");
	
	//check if new values are within legal range (within grid)
	if(y_new < 0 || y_new >= grid->y ||
	   x_new < 0 || x_new >= grid->x){
		return ILLEGAL;
	}
	printf("move_object: move is inside field\n");
	//get objects and squares for new and current position
	current_object = get_Playgrid_object(x,y,grid);
        
    new_square = get_Playgrid_square(x_new,y_new,grid);

	//if new_square is a wall, not legal move
	switch(new_square){
		case WALL: { return ILLEGAL; break; }
		case GOAL:case NORMAL:
		default: break;
	}
	printf("move_object: move does not walk into wall\n");
	
	//move depends on object being moved
	switch(current_object){
		case EMPTY:	//cannot move something empty
			printf("move_object: moving EMPTY\n");
			return ILLEGAL;		
		break;	
		case PLAYER:
			printf("move_object: moving PLAYER\n");
			return move_player(direction, x_new, y_new, grid,updated);
		break;	
		case ROCK:
			printf("move_object: moving ROCK\n");
			return move_rock(x,y,x_new,y_new,grid,updated);		
		break;
		
		default:
			printf("moving object, DEFAULT CASE\n");
			return ILLEGAL;		
		break;
	}
	printf("Not moving anything???\n");
	return LEGAL;
}

playgrid_legal move_rock(int x,int y, int x_new, int y_new, PlayGrid* grid, Square_list* updated){
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
	update_square( grid, EMPTY,x, y, updated);
	update_square( grid, ROCK,x_new, y_new, updated);


	return LEGAL;
}

playgrid_legal move_player(playgrid_direction direction, int x_new, int y_new, PlayGrid* grid, Square_list* updated){
	playgrid_object new_object;	//object at new square
	
	new_object = get_Playgrid_object(x_new,y_new,grid);
	
	//is the new square legal to move to	
	switch(new_object){
		case ROCK://try to move the rock
			if( ILLEGAL == move_object(x_new, y_new, direction, grid, updated) ){
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

	update_square( grid, EMPTY,grid->player_x, grid->player_y, updated);
	update_square( grid, PLAYER,x_new, y_new, updated);
	return LEGAL;
}

