#ifndef PLAYGRID_H
#define PLAYGRID_H
typedef enum {GOAL, NORMAL, WALL} playgrid_square;
typedef enum {EMPTY, ROCK, PLAYER} playgrid_object;
typedef enum {RIGHT, LEFT, UP, DOWN, NONE} playgrid_direction;
typedef enum {LEGAL, ILLEGAL} playgrid_legal;
typedef struct
{
	int x;
	int y;

	int player_x;
	int player_y;

	int n_rocks;
	playgrid_object** object_grid;
	playgrid_square** square_grid;
}PlayGrid;

typedef struct{
	int n;
	int x[4];
	int y[4];
}Square_list;

PlayGrid* init_playGrid();
void remove_playGrid(PlayGrid* grid);

playgrid_square get_Playgrid_square(int x,int y, PlayGrid* grid);
playgrid_object get_Playgrid_object(int x, int y, PlayGrid* grid);

playgrid_legal move_object(int x, int y, playgrid_direction direction, PlayGrid* grid, Square_list* updated);

int n_rocks_in_goal(PlayGrid* grid);

#endif 
