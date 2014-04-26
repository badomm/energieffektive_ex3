#include "screen.h"
#include <stdlib.h>

#define SCREEN "/dev/fb0"


static short* memory_map_screen;
static int screen_file_ID;

typedef short (*color_function)(int, int);

void update_screen_area(int dx, int dy, int width, int height);

void init_screen(){
	screen_file_ID = open(SCREEN, O_RDWR);
	memory_map_screen = mmap(0, SCREENSIZE, PROT_WRITE, MAP_SHARED, screen_file_ID, 0);
	printf("Screen initialized\n");
}

void clean_screen(){
	munmap(memory_map_screen, SCREENSIZE);
	//close(screen_file_ID);
	printf("Clean screen\n");
}

void write2pixel(int x, int y, short color){
	*(memory_map_screen + y*SCREEN_X + x) = color;
}

void update_screen_area(int i_dx, int i_dy, int i_width, int i_height) {
    struct fb_copyarea rect;

    rect.dx = i_dx;
    rect.dy = i_dy;

    rect.width  = i_width;
    rect.height = i_height;
    
    ioctl(screen_file_ID, 0x4680, &rect);
}


void fill_screen(short color){
	int x,y;	
	for(x = 0; x < SCREEN_X; x++){
		for(y = 0; y < SCREEN_Y; y++){
			write2pixel(x,y,color);
		}		
	}
	update_screen_area(0,0,SCREEN_X,SCREEN_Y);
}




