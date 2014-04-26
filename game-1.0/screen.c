#include "screen.h"

#define SCREEN "/dev/fb0"
#define RED    0xF800
#define GREEN  0x07E0
#define BLUE   0x001F
#define YELLOW 0xFFE0
#define BLACK  0x0000
#define WHITE  0xFFFF
#define CYAN   0x07FF
#define PINK   0xf81f

typedef short (*color_function)(int, int);

struct fb_copyarea update_screen_area(int dx, int dy, int width, int height);
struct fb_copyarea write_entire_screen(short* map, color_function f);


struct fb_copyarea write_first_grid(short* map, PlayGrid grid);



void print_grid(PlayGrid grid) {
	printf("I cant do that!\n");
	printf("%c\n", grid.object_grid[0][0]);

	char *screen = SCREEN;
	int fbfd = open(screen, O_RDWR);

	short* map = mmap(0, SCREENSIZE, PROT_WRITE, MAP_SHARED, fbfd, 0);

	struct fb_copyarea rect = write_first_grid(map, grid);
	ioctl(fbfd, 0x4680, &rect);
}

short sokoban_color(PlayGrid grid, int pixel_i, int pixel_j) {
	int x_tile_width = 240 / grid.y; // TODO x og y er stygt
	int y_tile_width = 320 / grid.x; // TODO samme

	int x = pixel_i / x_tile_width;
	int y = pixel_j / y_tile_width;

	if ((y == grid.player_x) && (x == grid.player_y)) {
		if (grid.square_grid[y][x] == NORMAL)
			return BLUE;
		else if (grid.square_grid[y][x] == GOAL)
			return BLACK;
	}
	if (grid.square_grid[y][x] == WALL)
		return RED;
	if (grid.square_grid[y][x] == GOAL) {
		if (grid.object_grid[y][x] == EMPTY)
			return YELLOW;
		else if (grid.object_grid[y][x] == ROCK)
			return GREEN;
	}
	if (grid.square_grid[y][x] == NORMAL) {
		if (grid.object_grid[y][x] == EMPTY)
			return PINK;
		else if (grid.object_grid[y][x] == ROCK)
			return CYAN;
	}
	return WHITE;
}

struct fb_copyarea write_first_grid(short* map, PlayGrid grid) {
	int i, j;
	short color;

	for (i = 0; i < 240; i++) {
		for (j = 0; j < 320; j++) {
			color = sokoban_color(grid, i, j);
			*(map + i*320 + j) = color;
		}
	}
	return update_screen_area(0, 0, 320, 240);
}

struct fb_copyarea update_screen_area(int i_dx, int i_dy, int i_width, int i_height) {
    struct fb_copyarea rect;

    rect.dx = i_dx;
    rect.dy = i_dy;

    rect.width  = i_width;
    rect.height = i_height;
    
    return rect; // TODO: User ioctl directly here, instead of returning.
}

struct fb_copyarea write_entire_screen(short* map, color_function f) {
    int i, j;
    short color;

    for (i = 0; i < 240; i++) {
        for (j = 0; j < 320; j++) {
            color = (*f)(i, j);
            *(map+i*320+j) = color;
        }
    }
    return update_screen_area(0, 0, 320, 240);
}


