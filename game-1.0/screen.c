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
short checkered_board(int i, int j);
short red_screen(int i, int j);
short green_screen(int i, int j);
short blue_screen(int i, int j);
short yellow_screen(int i, int j);
short black_screen(int i, int j);
short white_screen(int i, int j);

struct fb_copyarea write_first_grid(short* map, PlayGrid grid);

void screen_tests() {    
    printf("screenwriting\n");

    char *screen = SCREEN;
    int fbfd     = open(screen,O_RDWR);

    short* map   = mmap(0, SCREENSIZE, PROT_WRITE, MAP_SHARED, fbfd, 0);
    
    color_function f = &green_screen;
    struct fb_copyarea rect = write_entire_screen(map, f);
    ioctl(fbfd, 0x4680, &rect);

	printf("screenwrote\n");
}

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
	int x_tile_width = 240 / grid.y;
	int y_tile_width = 320 / grid.x;

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

short checkered_board(int i, int j) {
    int r, c;
    short color;
    
    r = i / 32;
    c = j / 16;
    color = (r%2) == (c%2) ? BLUE : GREEN;
            
    if (i/32 == 0 || j/16 == 0 || i/32 == 7 || j/16 == 19)
        color = RED;
    return color;
}

short red_screen(int i, int j) {
    return RED;
}

short green_screen(int i, int j) {
    return GREEN;
}

short blue_screen(int i, int j) {
    return BLUE;
}

short yellow_screen(int i, int j) {
	return YELLOW;
}

short black_screen(int i, int j) {
	return BLACK;
}

short white_screen(int i, int j) {
	return WHITE;
}
