#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/fb.h>
#include <sys/mman.h>

#include "playgrid.h"
#define SCREENSIZE (320*240)
#define SCREEN_X 320
#define SCREEN_Y 240


void init_screen();
void clean_screen();

void write2pixel(int x, int y, short color);
void update_screen_area(int i_dx, int i_dy, int i_width, int i_height);

void fill_screen(short color);

#endif

