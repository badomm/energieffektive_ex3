#include "sokoban.h"

#define cPLAYER '@'
#define cWALL   '#'
#define cEMPTY  ' '
#define cTARGET '.'
#define cPLAYTA '+'
#define cBOX    '$'
#define cBOXTA  '*' 

void game_tests() {
	printf("lololololol\n");
	int c, nl;
	int x, y, player_x, player_y, n_rocks;

	x = 10;
	y = 8;

	player_x = 7;
	player_y = 2;

	n_rocks = 4;

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
	object_grid[4][6] = ROCK;
	object_grid[5][6] = ROCK;
	object_grid[6][6] = ROCK;


	square_grid[2][5] = WALL;
	square_grid[3][5] = WALL;

	square_grid[6][6] = GOAL;
	square_grid[5][5] = GOAL;
	square_grid[4][4] = GOAL;
	square_grid[3][3] = GOAL;

	printf("Loooooooool 3 4: %d,\t 6 6: %d,\t 2 2: %d\t WALL: 2?: %d\n", object_grid[3][4], object_grid[6][6], object_grid[2][2], square_grid[2][5]);

	for (i = 0; i < y; i++) {
		for (j = 0; j < x; j++) {
			if (j == player_x && i == player_y) {
				if (square_grid[j][i] == GOAL)
					printf("%c", cPLAYTA);
				else
					printf("%c", cPLAYER);
				continue;
			}
			if (square_grid[j][i] == WALL)
				printf("%c", cWALL);
			if (square_grid[j][i] == NORMAL)
				printf("%c", cEMPTY);
			if (square_grid[j][i] == GOAL) {
				if (object_grid[j][i] == EMPTY)
					printf("%c", cTARGET);
				else
					printf("%c", cBOX);
			}
		}
		printf("\n");
	}
	

	PlayGrid grid = {x, y, player_x, player_y, n_rocks, object_grid, square_grid};

	print_grid(grid);

/*
	PlayGrid grid = {
		10, // x
		10, // y
		7,  // player_x
		2,  // player_y
		4,  // n_rocks
		{   // object_grid
			{EMPTY, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY},
			{EMPTY, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY},
			{EMPTY, EMPTY,  EMPTY,  ROCK ,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY},
			{EMPTY, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  ROCK ,  EMPTY,  EMPTY},
			{EMPTY, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  ROCK ,  EMPTY,  EMPTY},
			{EMPTY, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  ROCK ,  EMPTY,  EMPTY},
			{EMPTY, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY},
			{EMPTY, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY},
			{EMPTY, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY},
			{EMPTY, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY},
		},
		{   // square_grid
			{WALL,  WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL},
			{WALL,  NORMAL, NORMAL, NORMAL, WALL  , NORMAL, NORMAL, NORMAL, NORMAL, WALL},
			{WALL,  NORMAL, NORMAL, NORMAL, WALL  , NORMAL, NORMAL, NORMAL, NORMAL, WALL},
			{WALL,  NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, WALL},
			{WALL,  GOAL  , NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, WALL},
			{WALL,  NORMAL, GOAL  , NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, WALL},
			{WALL,  NORMAL, NORMAL, GOAL  , NORMAL, GOAL  , NORMAL, NORMAL, NORMAL, WALL},
			{WALL,  NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, WALL},
			{WALL,  NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, WALL},
			{WALL,  WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL},
		}
	}
*/
	printf("Enter move sequence:\n");
	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			nl++;
			printf("Enter move sequence:\n");
		}
		if (c == 'u')
			printf("Elevator going up\n");
		if (c == 'd')
			printf("You're going down\n");
		if (c == 'l')
			printf("Lefty\n");
		if (c == 'r')
			printf("Right\n");
		if (c == 'n')
			printf("New game\n");
	}
	printf("%d\n", nl);
}
