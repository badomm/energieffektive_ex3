#include "screen.h"

#define SCREEN "/dev/fb0"
#define RED   0xF800
#define GREEN 0x07e0
#define BLUE  0x001F

typedef short (*color_function)(int, int);

struct fb_copyarea update_screen_area(int dx, int dy, int width, int height);
struct fb_copyarea write_entire_screen(short* map, color_function f);
short checkered_board(int i, int j);
short red_screen(int i, int j);
short green_screen(int i, int j);
short blue_screen(int i, int j);


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

